#!/bin/bash
# 4) Display a special formatted listing showing the permissions, size,
#    filename, last modification time and last access time of filenames
#    supplied as arguments.

if [ $# -eq 0 ]
then
    echo "Usage: $0 filename [filename ...]"
    exit 1
fi

echo "=========================================================================================="
printf "%-12s %-8s %-18s %-22s %-22s\n" "Permissions" "Size" "Filename" "Last Modified" "Last Accessed"
echo "=========================================================================================="

for file in "$@"
do
    if [ ! -e "$file" ]
    then
        echo "$file: not found"
        continue
    fi

    perm=`ls -l "$file" | awk '{print $1}'`
    size=`ls -l "$file" | awk '{print $5}'`
    name=`basename "$file"`
    mtime=`ls -l "$file" | awk '{print $6, $7, $8}'`
    atime=`ls -lu "$file" | awk '{print $6, $7, $8}'`

    printf "%-12s %-8s %-18s %-22s %-22s\n" "$perm" "$size" "$name" "$mtime" "$atime"
done
