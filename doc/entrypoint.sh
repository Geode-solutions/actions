#!/bin/sh -l

ls 
cd $1
clang-doc-16  --output=doc --doxygen --public --format=md --executor=all-TUs compile_commands.json
python3 style.py .