#!/usr/bin/env bash
# Tests for fingerprint.sh, with `gh` stubbed from JSON fixtures.
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(mktemp -d)"
trap 'rm -rf "$ROOT"' EXIT
FAILED=0

mkdir -p "$ROOT/bin"
cat > "$ROOT/bin/gh" <<'STUB'
#!/usr/bin/env bash
# gh api <path> --jq <expr>  ->  jq -r <expr> $FIXTURES/<path, / -> _>.json
[ "$1" = "api" ] || exit 2
path="$2"
expr="$4"
file="$FIXTURES/${path//\//_}.json"
[ -f "$file" ] || exit 1
jq -r "$expr" "$file"
STUB
chmod +x "$ROOT/bin/gh"

# new_case: fresh fixtures dir and work dir
new_case() {
  CASE="$(mktemp -d -p "$ROOT")"
  export FIXTURES="$CASE/fixtures"
  mkdir -p "$FIXTURES" "$CASE/work"
}

# head_fixture <branch> <commit sha> <tree sha>
head_fixture() {
  echo "{\"sha\":\"$2\",\"commit\":{\"tree\":{\"sha\":\"$3\"}}}" \
    > "$FIXTURES/repos_Geode-solutions_OpenGeode_commits_$1.json"
}

# dep_fixture <owner/repo> <release id> <tag> <tree sha>
dep_fixture() {
  local base="${1//\//_}"
  echo "{\"tag_name\":\"$3\"}" > "$FIXTURES/repos_${base}_releases_$2.json"
  echo "{\"sha\":\"x\",\"commit\":{\"tree\":{\"sha\":\"$4\"}}}" \
    > "$FIXTURES/repos_${base}_commits_$3.json"
}

# selection <owner/repo> <release id> <updated_at> ...
selection_entry() {
  echo "\"$1\":{\"release_id\":$2,\"assets\":[{\"name\":\"a\",\"url\":\"u\",\"size\":1,\"updated_at\":\"$3\"}]}"
}

# run_fp <branch> <selection json>; sets KEY, LOG
run_fp() {
  rm -f "$CASE/work/out" "$CASE/work/schedule-fingerprint"
  LOG="$(cd "$CASE/work" && PATH="$ROOT/bin:$PATH" BRANCH="$1" SELECTION="$2" GH_TOKEN=t \
    GITHUB_REPOSITORY=Geode-solutions/OpenGeode GITHUB_OUTPUT="$CASE/work/out" \
    GITHUB_RUN_ID=42 bash "$HERE/fingerprint.sh" 2>&1)"
  RC=$?
  KEY="$(sed -n 's/^key=//p' "$CASE/work/out" 2>/dev/null)"
}

assert() { # name, condition result
  if [ "$2" = "0" ]; then echo "PASS $1"; else echo "FAIL $1"; FAILED=1; fi
}

SEL_TWO='{"A/dep1":{"release_id":1,"assets":[{"name":"a","url":"u","size":1,"updated_at":"2024-01-01T00:00:00Z"}]},"B/dep2":{"release_id":2,"assets":[{"name":"a","url":"u","size":1,"updated_at":"2024-02-01T00:00:00Z"}]}}'

test_same_trees_same_key() {
  new_case
  head_fixture master aaa T1; dep_fixture A/dep1 1 v33.33.0-rc.1 D1; dep_fixture B/dep2 2 v33.33.0 D1
  run_fp master "$SEL_TWO"; local k1="$KEY"
  head_fixture master bbb T1
  run_fp master "$SEL_TWO"
  [ -n "$k1" ] && [ "$k1" = "$KEY" ]; assert test_same_trees_same_key $?
}

test_head_tree_change_changes_key() {
  new_case
  head_fixture master aaa T1; dep_fixture A/dep1 1 v33.33.0-rc.1 D1; dep_fixture B/dep2 2 v33.33.0 D1
  run_fp master "$SEL_TWO"; local k1="$KEY"
  head_fixture master aaa T2
  run_fp master "$SEL_TWO"
  [ -n "$k1" ] && [ -n "$KEY" ] && [ "$k1" != "$KEY" ]; assert test_head_tree_change_changes_key $?
}

test_dependency_tree_change_changes_key() {
  new_case
  head_fixture master aaa T1; dep_fixture A/dep1 1 v33.33.0-rc.1 D1; dep_fixture B/dep2 2 v33.33.0 D1
  run_fp master "$SEL_TWO"; local k1="$KEY"
  dep_fixture A/dep1 1 v33.33.0-rc.1 D2
  run_fp master "$SEL_TWO"
  [ -n "$k1" ] && [ -n "$KEY" ] && [ "$k1" != "$KEY" ]; assert test_dependency_tree_change_changes_key $?
}

test_branch_not_in_key() {
  new_case
  head_fixture master aaa T1; head_fixture next aaa T1
  dep_fixture A/dep1 1 v33.33.0-rc.1 D1; dep_fixture B/dep2 2 v33.33.0 D1
  run_fp next "$SEL_TWO"; local k1="$KEY"
  run_fp master "$SEL_TWO"
  [ "$k1" = "$KEY" ] && [[ "$KEY" == schedule-tree-* ]]; assert test_branch_not_in_key $?
}

test_selection_order_independent() {
  new_case
  head_fixture master aaa T1; dep_fixture A/dep1 1 v33.33.0-rc.1 D1; dep_fixture B/dep2 2 v33.33.0 D2
  local s1 s2
  s1="{$(selection_entry A/dep1 1 2024-01-01T00:00:00Z),$(selection_entry B/dep2 2 2024-02-01T00:00:00Z)}"
  s2="{$(selection_entry B/dep2 2 2024-02-01T00:00:00Z),$(selection_entry A/dep1 1 2024-01-01T00:00:00Z)}"
  run_fp master "$s1"; local k1="$KEY"
  run_fp master "$s2"
  [ -n "$k1" ] && [ "$k1" = "$KEY" ]; assert test_selection_order_independent $?
}

test_head_lookup_failure_degrades() {
  new_case # no head fixture
  run_fp master ""
  local k1="$KEY" rc1=$RC log1="$LOG"
  run_fp master ""
  [ "$rc1" = 0 ] && [[ "$k1" == schedule-tree-* ]] && [[ "$log1" == *"unresolved-42"* ]] \
    && [ "$k1" = "$KEY" ]; assert test_head_lookup_failure_degrades $?
}

test_non_release_tag_falls_back_to_build_id() {
  new_case
  head_fixture master aaa T1; dep_fixture A/dep1 1 nightly D1
  local s1 s2
  s1="{$(selection_entry A/dep1 1 2024-01-01T00:00:00Z)}"
  s2="{$(selection_entry A/dep1 1 2024-03-01T00:00:00Z)}"
  run_fp master "$s1"; local k1="$KEY" log1="$LOG"
  run_fp master "$s2"
  [ "$k1" != "$KEY" ] && [[ "$log1" != *"D1"* ]]; assert test_non_release_tag_falls_back_to_build_id $?
}

test_dependency_lookup_failure_falls_back() {
  new_case
  head_fixture master aaa T1 # no release fixture for A/dep1
  local s1 s2
  s1="{$(selection_entry A/dep1 1 2024-01-01T00:00:00Z)}"
  s2="{$(selection_entry A/dep1 2 2024-01-01T00:00:00Z)}"
  run_fp master "$s1"; local k1="$KEY" rc1=$RC
  run_fp master "$s2"
  [ "$rc1" = 0 ] && [[ "$k1" == schedule-tree-* ]] && [ "$k1" != "$KEY" ]; assert test_dependency_lookup_failure_falls_back $?
}

test_empty_selection_only_head() {
  new_case
  head_fixture master aaa T1
  run_fp master ""; local k1="$KEY" rc1=$RC
  run_fp master "{}"
  local expected
  expected="$(printf '%s' 'OpenGeode:T1' | sha256sum | cut -d' ' -f1)"
  [ "$rc1" = 0 ] && [ "$k1" = "schedule-tree-$expected" ] && [ "$KEY" = "$k1" ]; assert test_empty_selection_only_head $?
}

test_hash_file_written() {
  new_case
  head_fixture master aaa T1; dep_fixture A/dep1 1 v1.0.0 D1
  run_fp master "{$(selection_entry A/dep1 1 2024-01-01T00:00:00Z)}"
  local h
  h="$(cat "$CASE/work/schedule-fingerprint" 2>/dev/null)"
  [[ "$h" =~ ^[0-9a-f]{64}$ ]] && [ "$KEY" = "schedule-tree-$h" ]; assert test_hash_file_written $?
}

test_log_lines() {
  new_case
  head_fixture master aaa T1; dep_fixture A/dep1 1 v1.0.0 D1
  run_fp master "{$(selection_entry A/dep1 1 2024-01-01T00:00:00Z)}"
  [[ "$LOG" == *"head: aaa tree T1"* ]] && [[ "$LOG" == *"dependency: A/dep1 v1.0.0 tree D1"* ]]; assert test_log_lines $?
}

for t in $(declare -F | awk '{print $3}' | grep '^test_'); do "$t"; done
exit "$FAILED"
