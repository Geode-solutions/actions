#!/bin/bash
set -e

third_party_path="$GITHUB_WORKSPACE/doxyrest"
mkdir -p $third_party_path
cd $third_party_path
doxyrest_name="doxyrest-2.0.0-linux-amd64"
wget https://github.com/vovkos/doxyrest/releases/download/doxyrest-2.0.0/$doxyrest_name.tar.xz
tar -xJf $doxyrest_name.tar.xz
doxyrest_path="$third_party_path/$doxyrest_name"

cd "$RUNNER_WORKSPACE/$INPUT_DIRECTORY"
cmake .
cmake --build . --target doc
cd doc
$doxyrest_path/bin/doxyrest xml/index.xml -c $doxyrest_path/share/doxyrest/frame/doxyrest-config.lua -F $doxyrest_path/share/doxyrest/frame/common -F $doxyrest_path/share/doxyrest/frame/cfamily 
pip3 install -U sphinx
export PYTHONPATH=$doxyrest_path/share/doxyrest/sphinx:$PYTHONPATH
sphinx-build -b html -c /doc rst site

docs_path="$third_party_path/docs"
repo_name=${GITHUB_REPOSITORY##*/}
git clone https://github.com/Geode-solutions/docs $docs_path
rm -rf $docs_path/static/docs/$repo_name
mkdir -p $docs_path/static/docs/$repo_name
cp -r ./site/* $docs_path/static/docs/$repo_name
cd $docs_path
git config user.email $GITHUB_ACTOR@users.noreply.github.com
git config user.name $GITHUB_ACTOR
git checkout master
git add --all
git commit -m "Update $repo_name documentation"
git push https://BotellaA:$GITHUB_TOKEN@github.com/Geode-solutions/docs
