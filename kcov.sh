#!/bin/bash
set -e

sudo apt update
sudo apt install pkg-config binutils-dev build-essential libcurl4-openssl-dev libdw-dev libiberty-dev python zlib1g-dev
        
git clone https://github.com/SimonKagstrom/kcov kcov_src
cd kcov_src
cmake .
cmake --build . -- -j2
sudo cmake --build . --target install

OUTPUT_DIRECTORY="${GITHUB_WORKSPACE}/kcov"
mkdir -p $OUTPUT_DIRECTORY
cd $1

for test in test-*
do
    kcov --include-path="${GITHUB_WORKSPACE}/include","${GITHUB_WORKSPACE}/src" --exclude-pattern=common.cpp $OUTPUT_DIRECTORY $test
done

ls $OUTPUT_DIRECTORY
TEST_PATH="$(ls -d $OUTPUT_DIRECTORY/*/)"
TEST_PATH2="$(find -type d -mindepth 1 -maxdepth 1  $OUTPUT_DIRECTORY/*/)"
NB_TESTS="$(find $OUTPUT_DIRECTORY/test-* | wc -l)"
echo $TEST_NAME
echo $NB_TESTS
if [ ${NB_TESTS} -eq "1" ]; then 
    cp $TEST_PATH/cobertura.xml $OUTPUT_DIRECTORY/kcov-merged;
fi
