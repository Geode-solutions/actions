#!/bin/bash
set -e

OUTPUT_DIRECTORY="${GITHUB_WORKSPACE}/kcov"
mkdir -p $OUTPUT_DIRECTORY
cd $1

for test in test-*
do
    kcov --include-path="${GITHUB_WORKSPACE}/include","${GITHUB_WORKSPACE}/src" --exclude-pattern=common.cpp $OUTPUT_DIRECTORY $test
done

TEST_PATH="$(find $OUTPUT_DIRECTORY/test-* -maxdepth 0 -type d)"
echo $TEST_PATH
NB_TESTS="$(find $1/test-* | wc -l)"
echo $NB_TESTS
if [ ${NB_TESTS} -eq "1" ]; then 
    cp $TEST_PATH/cobertura.xml $OUTPUT_DIRECTORY/kcov-merged;
fi
