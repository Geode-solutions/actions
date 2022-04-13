#!/bin/bash
set -e

wget https://raw.githubusercontent.com/Geode-solutions/actions/master/clang-format/.clang-format -O .clang-format
pwd
ls -a
files=$(find . \( -name "*.h" -o -name "*.cpp" \))
clang-format-12 -style=file -i $files

changes=$(git diff)
if [ -n "$changes" ]; then
    git config user.email $GITHUB_ACTOR@users.noreply.github.com
    git config user.name $GITHUB_ACTOR
    branch=`echo ${GITHUB_REF#*/*/}`
    echo $branch
    git checkout $branch
    git add --all
    git commit -m "style: CI format update"
    git push https://$1:$GITHUB_TOKEN@github.com/$GITHUB_REPOSITORY HEAD:$branch
    echo ::error::Format requirement is not satisfied
    exit 1
fi
