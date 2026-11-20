#!/usr/bin/env bash
# Fingerprint of what the scheduled run would test: the git tree of the repository head
# plus the git tree of each dependency release.
#
# Trees are used rather than commit shas because a master release is a promotion: its tag points
# at a commit with the same git tree as the release candidate the nightly already tested.
# Hashing the tree makes the promotion a no-op, while any real source change still changes the key.
#
# A failed lookup never fails the script. It degrades to a value that changes the key
# (unresolved head, build id of the release), so the worst case is an unneeded run.

BRANCH="${BRANCH:?}"
SELECTION="${SELECTION:-}"
[ -n "$SELECTION" ] || SELECTION='{}'

head_sha=""
head_tree=""
read -r head_sha head_tree < <(
  gh api "repos/$GITHUB_REPOSITORY/commits/$BRANCH" --jq '"\(.sha) \(.commit.tree.sha)"' 2> /dev/null
) || true
if [ -z "$head_tree" ]; then
  head_sha="unresolved"
  head_tree="unresolved-$GITHUB_RUN_ID"
fi
echo "head: $head_sha tree $head_tree"

material="${GITHUB_REPOSITORY#*/}:$head_tree"

tag_regex='^v[0-9]+\.[0-9]+\.[0-9]+(-rc\.[0-9]+)?$'
entries=$(jq -r 'to_entries | sort_by(.key)[] | "\(.key) \(.value.release_id) \([.value.assets[].updated_at] | max // "")"' <<< "$SELECTION" 2> /dev/null) || entries=""

while read -r repo release_id updated_at; do
  [ -n "$repo" ] || continue
  tree=""
  tag=$(gh api "repos/$repo/releases/$release_id" --jq .tag_name 2> /dev/null) || tag=""
  if [[ "$tag" =~ $tag_regex ]]; then
    tree=$(gh api "repos/$repo/commits/$tag" --jq .commit.tree.sha 2> /dev/null) || tree=""
  fi
  if [ -n "$tree" ]; then
    echo "dependency: $repo $tag tree $tree"
    material="$material;$repo:$tree"
  else
    echo "dependency: $repo ${tag:-unknown} build $release_id"
    material="$material;$repo:build-$release_id:$updated_at"
  fi
done <<< "$entries"

hash=$(printf '%s' "$material" | sha256sum | cut -d' ' -f1)
echo "key=schedule-tree-$hash" >> "$GITHUB_OUTPUT"
echo "$hash" > schedule-fingerprint
exit 0
