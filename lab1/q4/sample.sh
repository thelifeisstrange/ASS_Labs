#!/bin/bash
# 4) Write a shell script to display all the lines in a single file in the
#    current directory having manipal as a word.

echo "Enter file name:"
read filename

if [ ! -f "$filename" ]
then
    echo "File not found"
    exit 1
fi

echo "Lines containing the word manipal:"
grep -n -w "manipal" "$filename"
