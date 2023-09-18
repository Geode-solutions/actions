#!/bin/sh -l

clang-doc --version
cd build/$INPUT_DIRECTORY
clang-doc  --output=doc --doxygen --public --format=md --executor=all-TUs compile_commands.json
python3 /style.py .