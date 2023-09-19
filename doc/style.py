# -*- coding: utf-8 -*-
import sys
import os

try:
    from StringIO import StringIO
except ImportError:
    from io import StringIO

LIST_MARKERS = ("* ", "- ", "+ ")


def bake_bucket(output, bucket, directory):
    for item in sorted(bucket):
        filename = item[item.find("(") + 1: item.find(")")]
        file = os.path.join(directory, filename)
        if os.path.exists(file):
            output.write(item)


def sort(stream, output, directory):
    buckets = set()

    for line in stream:
        stripped = line.lstrip()

        if not stripped:
            if buckets:
                bake_bucket(output, buckets, directory)
                buckets.clear()
            output.write(line)
            continue

        if stripped[:2] in LIST_MARKERS:
            buckets.add(line)
        elif stripped[:11] == "*Defined at":
            continue
        elif stripped[0] == "*" and stripped[-2] == "*":
            output.write("```cpp\n")
            output.write(stripped[1:-2])
            output.write("\n```\n")
        elif stripped[:7] == "public " or stripped[:10] == "protected ":
            output.write("```cpp\n")
            output.write(line)
            output.write("```\n")
        elif stripped[0] == "#" and "<" in stripped and ">" in stripped:
            output.write(stripped.split("<", 1)[0])
            output.write("\n")
        elif stripped[:13] == "Inherits from":
            output.write("```cpp\n")
            output.write(line)
            output.write("```\n")
        else:
            output.write(line)

    # for remains
    if buckets:
        bake_bucket(output, buckets, directory)


def sort_string(directory, string):
    stream = StringIO(string)
    output = StringIO()
    sort(stream, output, directory)
    return output.getvalue()


def rewrite_file(directory, filename):
    input = open(filename, "r")
    source = input.read()
    result = sort_string(directory, source)
    input.close()
    with open(input.name, "w", encoding=input.encoding) as output:
        output.write(result)


def clean_directory(directory):
    for filename in os.listdir(directory):
        f = os.path.join(directory, filename)
        if os.path.isdir(f):
            clean_directory(f)
            continue
        if filename == "Impl.md":
            os.remove(f)
            continue
        file = open(f, "r")
        if len(file.readlines()) < 3:
            os.remove(f)
            continue


def process_directory(directory):
    for filename in os.listdir(directory):
        f = os.path.join(directory, filename)
        if os.path.isdir(f):
            process_directory(f)
            continue
        rewrite_file(directory, f)


if __name__ == "__main__":
    args = sys.argv[1:]
    print(args)
    clean_directory(args[0])
    process_directory(args[0])
