#!/bin/bash
set -e

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
