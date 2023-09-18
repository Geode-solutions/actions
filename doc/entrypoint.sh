#!/bin/sh -l

pwd
echo "base=$INPUT_BASE_DIRECTORY"
cd build/$INPUT_DIRECTORY
ls
head compile_commands.json
sed -i -E  "s~$INPUT_BASE_DIRECTORY~/github/workspace~g" compile_commands.json
head compile_commands.json
clang-doc-18  --output=doc --doxygen --public --format=md --executor=all-TUs compile_commands.json
cd doc/geode
python3 /style.py .