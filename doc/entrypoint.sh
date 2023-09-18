#!/bin/sh -l

directory = $1
base_directory = $2
pwd
echo "base=$base_directory"
cd build/$directory
ls
head compile_commands.json
sed -i -E "s~$base_directory~/github/workspace~g" compile_commands.json
head compile_commands.json
clang-doc-18  --output=doc --doxygen --public --format=md --executor=all-TUs compile_commands.json
cd doc/geode
python3 /style.py .