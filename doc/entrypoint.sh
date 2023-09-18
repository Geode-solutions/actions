#!/bin/sh -l

cd build/$INPUT_DIRECTORY
clang-doc-18  --output=doc --doxygen --public --format=md --executor=all-TUs compile_commands.json
cd doc/geode
python3 /style.py .