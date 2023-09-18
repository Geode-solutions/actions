#!/bin/sh -l

pwd
cd build/$INPUT_DIRECTORY
ls
head compile_commands.json
clang-doc-18  --output=doc --doxygen --public --format=md --executor=all-TUs compile_commands.json
cd doc/geode
python3 /style.py .