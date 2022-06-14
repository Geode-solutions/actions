#!/bin/bash
set -e

git config --global --add safe.directory /github/workspace
changes=$(git diff)
if [ -n "$changes" ]; then
    git config user.email $GITHUB_ACTOR@users.noreply.github.com
    git config user.name $GITHUB_ACTOR
    branch=`echo ${GITHUB_REF#*/*/}`
    echo $branch
    git checkout $branch
    git add --all
    git commit -m "update versions"
    git push https://$1:$GITHUB_TOKEN@github.com/$GITHUB_REPOSITORY HEAD:$branch
    echo ::error::Format requirement is not satisfied
    exit 1
fi
