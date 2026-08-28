#!/bin/bash
# 3) Write a shell script to display the current date and time and also
#    the number of users logged into the system.

echo "Current date and time:"
date

echo
echo "Number of users logged into the system:"
who | wc -l

echo
echo "Logged in users:"
who
