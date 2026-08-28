#!/bin/bash
# 5) Write a Shell Script that accepts a file name, starting and ending
#    line numbers as arguments and displays all lines between the
#    given line numbers.

if [ $# -ne 3 ]
then
    echo "Usage: $0 filename start_line end_line"
    exit 1
fi

filename=$1
start=$2
end=$3

if [ ! -f "$filename" ]
then
    echo "File not found"
    exit 1
fi

echo "Lines from $start to $end in $filename:"
sed -n "$start,${end}p" "$filename"
