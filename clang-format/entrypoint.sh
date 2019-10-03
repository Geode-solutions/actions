#!/bin/sh
set -e

cd "${GITHUB_WORKSPACE}"
wget https://raw.githubusercontent.com/Geode-solutions/actions/master/clang-format/.clang-format -O .clang-format

files=$(find . \( -name "*.h" -o -name "*.cpp" \))
clang-format-8 -style=file -i $files

if ! git diff-index --quiet HEAD --; then
    git config user.email "${GITHUB_ACTOR}@users.noreply.github.com"
    git config user.name "${GITHUB_ACTOR}"
    git checkout `echo ${GITHUB_REF##*/*/}`
    git add --all "${GITHUB_WORKSPACE}"
    git commit -m "style: CI format update"
    git push origin
    echo ::error::Format requirement is not satisfied
fi
