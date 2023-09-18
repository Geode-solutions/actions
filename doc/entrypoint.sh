#!/bin/sh -l

directory=$1
base_directory=$2
cd build/$directory
sed -i -E "s~$base_directory~/github/workspace~g" compile_commands.json
clang-doc-18  --output=doc --doxygen --public --format=md --executor=all-TUs compile_commands.json
cd doc/geode
python3 /style.py .

docs_path="/docs"
repo_name=${GITHUB_REPOSITORY##*/}
repo_name="${repo_name%_private}"
repo_name="${repo_name,,}"
git clone https://github.com/Geode-solutions/docs $docs_path
doc_folder=$docs_path/references/$repo_name
rm -rf $doc_folder
mkdir -p $doc_folder
cp -r . $doc_folder
cd $docs_path


changes=$(git status --porcelain 2>/dev/null | wc -l)
if [ "$changes" -ne "0" ]; then
    git config user.email $GITHUB_ACTOR@users.noreply.github.com
    git config user.name $GITHUB_ACTOR
    git checkout master
    git add --all
    git commit -m "Update $repo_name documentation"
    git push https://BotellaA:$GITHUB_TOKEN@github.com/Geode-solutions/docs2
fi