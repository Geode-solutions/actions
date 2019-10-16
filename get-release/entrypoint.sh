#!/bin/sh
set -e

RELEASE_REPOSITORY="Geode-solutions/$INPUT_REPOSITORY"
TARGET_PATH="$GITHUB_WORKSPACE/$INPUT_REPOSITORY"
RELEASE_URL="https://api.github.com/repos/$RELEASE_REPOSITORY/releases"
OUTPUT_FILE="$INPUT_REPOSITORY$INPUT_FILE"

gh_curl() {
  if [ -z "$INPUT_TOKEN" ]; then
    curl -sL \
       -H "Accept: application/vnd.github.v3.raw" \
       "$@"
  else
    curl -sL -H "Authorization: token $INPUT_TOKEN" \
       -H "Accept: application/vnd.github.v3.raw" \
       "$@"
  fi
}

gh_get_asset() {
  if [ -z "$INPUT_TOKEN" ]; then
    curl -sL -H 'Accept: application/octet-stream' $RELEASE_URL/assets/$1
  else
    curl -sL -H "Authorization: token $INPUT_TOKEN" -H 'Accept: application/octet-stream' $RELEASE_URL/assets/$1
  fi
}

gh_extract_asset() {
  extension=`echo ${OUTPUT_FILE##*.}`
  if [ $extension = "zip" ]; then
    unzip $OUTPUT_FILE
    rm $OUTPUT_FILE
  elif [ $extension == "gz" ]; then
    tar xzf $OUTPUT_FILE
    rm $OUTPUT_FILE
  fi
}


mkdir -p "$TARGET_PATH"
cd "$TARGET_PATH"

PARSER=".assets | map(select(.name|test(\"$INPUT_FILE\")))[0].id"
ASSET_ID=`gh_curl $RELEASE_URL/latest | jq "$PARSER"`
if [ "$ASSET_ID" = "null" ]; then
  >&2 echo "ERROR: file not found '$FILE'"
  exit 1
fi

gh_get_asset $ASSET_ID > $OUTPUT_FILE

gh_extract_asset

folder=$(ls `pwd`)
echo ::set-output name=path::$INPUT_REPOSITORY/$folder

