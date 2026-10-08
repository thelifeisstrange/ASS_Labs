#!/bin/bash
# Extra 1) Two sorted numeric files -> one sorted file with no duplicates.

if [ $# -lt 2 ]
then
    echo "Usage: $0 file1 file2 [outfile]"
    exit 1
fi

f1=$1
f2=$2
out=${3:-merged.txt}

if [ ! -f "$f1" ] || [ ! -f "$f2" ]
then
    echo "Both input files must exist"
    exit 1
fi

sort -n -u "$f1" "$f2" > "$out"
echo "Merged sorted unique numbers written to $out"
cat "$out"
