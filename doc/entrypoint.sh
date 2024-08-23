#!/bin/bash

directory=$1
base_directory=$2
token=$3
repo_name=${GITHUB_REPOSITORY##*/}
repo_name="${repo_name%_private}"
repo_name="${repo_name,,}"
docs_path="/docs"

cd build/$directory
sed -i -E "s~$base_directory~/github/workspace~g" compile_commands.json
jq 'map(select(.file | test("/private/"; "i") | not))' compile_commands.json > public_compile_commands.json
clang-doc-20  --output=doc --doxygen --public --format=md --executor=all-TUs public_compile_commands.json
cd doc/geode
python3 /style.py .

git clone https://github.com/Geode-solutions/docs $docs_path
cd $docs_path
git checkout master
git pull 
doc_folder=$docs_path/references/$repo_name
rm -rf $doc_folder
mkdir -p $doc_folder
cp -r /github/workspace/build/$directory/doc/geode/* $doc_folder

changes=$(git status --porcelain 2>/dev/null | wc -l)
if [ "$changes" -ne "0" ]; then
    git config user.email $GITHUB_ACTOR@users.noreply.github.com
    git config user.name $GITHUB_ACTOR
    git add --all
    git commit -m "Update $repo_name documentation"
    git push https://$token@github.com/Geode-solutions/docs master
fi