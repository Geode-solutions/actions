#!/bin/bash
set -e

git clone https://github.com/SimonKagstrom/kcov kcov_src
cd kcov_src
cmake .
cmake --build . -- -j2
sudo cmake --build . --target install

OUTPUT_DIRECTORY="${GITHUB_WORKSPACE}/kcov"
mkdir -p $OUTPUT_DIRECTORY
cd $INPUT_DIRECTORY

for test in test-*
do
    kcov --include-path="${GITHUB_WORKSPACE}/include","${GITHUB_WORKSPACE}/src" --exclude-pattern=common.cpp $OUTPUT_DIRECTORY $test
done

TEST_NAME="$(ls $OUTPUT_DIRECTORY)"
NB_TESTS="$(find $OUTPUT_DIRECTORY/test-* | wc -l)"
if [ ${NB_TESTS} -eq "1" ]; then 
    cp $OUTPUT_DIRECTORY/$TEST_NAME/cobertura.xml $OUTPUT_DIRECTORY/kcov-merged;
fi
