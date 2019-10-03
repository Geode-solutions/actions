#!/bin/sh
set -e

cd "${GITHUB_WORKSPACE}"
wget https://raw.githubusercontent.com/Geode-solutions/actions/master/clang-format/.clang-format -O .clang-format

files=$(find . \( -name "*.h" -o -name "*.cpp" \))
clang-format-8 -style=file -i $files

if [[ `git status --porcelain` ]]; then
    echo ::error::Format requirement is not satisfied
fi
