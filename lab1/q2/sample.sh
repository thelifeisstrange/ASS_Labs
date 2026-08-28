#!/bin/bash
# 2) Write a shell script to accept a directory name and display all
#    files ending with .c.

echo "Enter directory name:"
read dirname

if [ ! -d "$dirname" ]
then
    echo "Directory does not exist"
    exit 1
fi

echo "Files ending with .c:"
ls "$dirname"/*.c
