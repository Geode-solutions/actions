#!/usr/bin/env bash
# Fingerprint of what the scheduled run would test: the git tree of the repository head
# plus the git tree of each dependency release.
#
# Trees are used rather than commit shas because a master release is a promotion: its tag points
# at a commit with the same git tree as the release candidate the nightly already tested.
# Hashing the tree makes the promotion a no-op, while any real source change still changes the key.
#
# When in doubt, the test runs: a failed or malformed lookup never fails the script, it degrades
# to a value that is unique to this run (or to the build id of the release), so the key never
# stays stable on bad data. The worst case is an unneeded run.

BRANCH="${BRANCH:?}"
SELECTION="${SELECTION:-}"
[ -n "$SELECTION" ] || SELECTION='{}'

tree_regex='^[0-9a-f]{40}$'
tag_regex='^v[0-9]+\.[0-9]+\.[0-9]+(-rc\.[0-9]+)?$'

head_sha=""
head_tree=""
read -r head_sha head_tree < <(
  gh api "repos/$GITHUB_REPOSITORY/commits/$BRANCH" --jq '"\(.sha) \(.commit.tree.sha)"' 2> /dev/null < /dev/null
) || true
if [[ ! "$head_tree" =~ $tree_regex ]]; then
  echo "::warning::Head of $BRANCH could not be resolved, the tests will run"
  head_sha="unresolved"
  head_tree="unresolved-$GITHUB_RUN_ID"
fi
echo "head: $head_sha tree $head_tree"

material="${GITHUB_REPOSITORY#*/}:$head_tree"

if entries=$(jq -r 'to_entries | sort_by(.key)[] | "\(.key) \(.value.release_id) \([(.value.assets // [])[].updated_at] | max // "")"' <<< "$SELECTION" 2> /dev/null); then
  while read -r repo release_id updated_at; do
    [ -n "$repo" ] || continue
    if [ -z "$release_id" ] || [ "$release_id" = "null" ]; then
      echo "::warning::Release of $repo is unresolved, the tests will run"
      material="$material;$repo:unresolved-$GITHUB_RUN_ID"
      continue
    fi
    tree=""
    tag=$(gh api "repos/$repo/releases/$release_id" --jq .tag_name 2> /dev/null < /dev/null) || tag=""
    if [[ "$tag" =~ $tag_regex ]]; then
      tree=$(gh api "repos/$repo/commits/$tag" --jq .commit.tree.sha 2> /dev/null < /dev/null) || tree=""
      [[ "$tree" =~ $tree_regex ]] || tree=""
      [ -n "$tree" ] || echo "::warning::Tree of $repo $tag could not be resolved, using its build id"
    elif [ -n "$tag" ] && [ "$tag" != "null" ]; then
      echo "::notice::$repo $tag is not a version, using its build id"
    else
      echo "::warning::Release $release_id of $repo could not be resolved, using its build id"
    fi
    if [ -n "$tree" ]; then
      echo "dependency: $repo $tag tree $tree"
      material="$material;$repo:$tree"
    elif [ -z "$updated_at" ]; then
      echo "::warning::$repo has no assets to identify its build, the tests will run"
      material="$material;$repo:unresolved-$GITHUB_RUN_ID"
    else
      echo "dependency: $repo ${tag:-unknown} build $release_id"
      material="$material;$repo:build-$release_id:$updated_at"
    fi
  done <<< "$entries"
else
  echo "::warning::Selection could not be parsed, the tests will run"
  material="$material;selection:unresolved-$GITHUB_RUN_ID"
fi

hash=$(printf '%s' "$material" | sha256sum | cut -d' ' -f1)
if [[ ! "$hash" =~ ^[0-9a-f]{64}$ ]]; then
  hash=$(printf '%s' "unresolved-$GITHUB_RUN_ID" | sha256sum | cut -d' ' -f1)
fi
echo "key=schedule-tree-$hash" >> "$GITHUB_OUTPUT"
echo "$hash" > schedule-fingerprint
exit 0
