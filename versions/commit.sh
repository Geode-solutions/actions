#!/bin/bash
set -e

git config --global --add safe.directory /github/workspace
changes=$(git diff)
if [ -n "$changes" ]; then
    git config user.email $GITHUB_ACTOR@users.noreply.github.com
    git config user.name $GITHUB_ACTOR
    branch=`echo ${GITHUB_REF#*/*/}`
    git add --all
    git commit -m "Update versions"
    git push https://BotellaA:$GITHUB_TOKEN@github.com/$GITHUB_REPOSITORY
fi