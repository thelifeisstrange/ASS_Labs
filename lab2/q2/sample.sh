#!/bin/bash
# 2) Accept two directory names d1 and d2, and delete those files
#    in d2 which have identical names in d1.

if [ $# -ne 2 ]
then
    echo "Usage: $0 d1 d2"
    exit 1
fi

d1=$1
d2=$2

if [ ! -d "$d1" ]
then
    echo "Directory $d1 does not exist"
    exit 1
fi

if [ ! -d "$d2" ]
then
    echo "Directory $d2 does not exist"
    exit 1
fi

echo "Files in $d1 before:"
ls "$d1"
echo
echo "Files in $d2 before:"
ls "$d2"
echo

for file in "$d1"/*
do
    if [ ! -f "$file" ]
    then
        continue
    fi

    fname=`basename "$file"`

    if [ -f "$d2/$fname" ]
    then
        rm "$d2/$fname"
        echo "Deleted $d2/$fname (same name exists in $d1)"
    fi
done

echo
echo "Files in $d2 after:"
ls "$d2"
