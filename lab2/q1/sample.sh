#!/bin/bash
# 1) Accept a string from the terminal and echo a suitable message
#    if it doesn't have at least 10 characters using case and expr.

echo "Enter a string:"
read str

len=`expr "$str" : '.*'`

case $len in
[0-9])
    echo "The string does not have at least 10 characters (length = $len)"
    ;;
*)
    echo "The string has 10 or more characters (length = $len)"
    ;;
esac
