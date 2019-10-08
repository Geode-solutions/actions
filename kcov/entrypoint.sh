#!/bin/bash
set -e

OUTPUT_DIR="${GITHUB_WORKSPACE}/kcov"
mkdir -p $OUTPUT_DIR
cd "$1"
ls


for test in test-*
do
    kcov $OUTPUT_DIR $test --include-path="${GITHUB_WORKSPACE}/include","${GITHUB_WORKSPACE}/src" --exclude-pattern=common.cpp
done

TEST_NAME="$(ls $OUTPUT_DIR)"
NB_TESTS="$(find $OUTPUT_DIR/test-* | wc -l)"
if [ ${NB_TESTS} -eq "1" ]; then 
    cp $OUTPUT_DIR/$TEST_NAME/cobertura.xml $OUTPUT_DIR/kcov-merged;
fi

ls $OUTPUT_DIR/kcov-merged
