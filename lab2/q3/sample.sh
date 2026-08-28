#!/bin/bash
# 3) Accept filenames as arguments. For every filename, check whether
#    it exists in the current directory and then convert its name to
#    uppercase, but only if a file with the new name doesn't exist.

if [ $# -eq 0 ]
then
    echo "Usage: $0 filename [filename ...]"
    exit 1
fi

for file in "$@"
do
    if [ ! -e "$file" ]
    then
        echo "$file does not exist in the current directory"
        continue
    fi

    newname=`echo "$file" | tr 'a-z' 'A-Z'`

    if [ "$file" = "$newname" ]
    then
        echo "$file is already in uppercase; no rename needed"
        continue
    fi

    # -ef is true when both names refer to the same file
    # (happens on case-insensitive filesystems like macOS)
    if [ -e "$newname" ] && [ ! "$file" -ef "$newname" ]
    then
        echo "Cannot rename $file to $newname: file with new name already exists"
    else
        mv "$file" "$newname"
        echo "Renamed $file to $newname"
    fi
done
