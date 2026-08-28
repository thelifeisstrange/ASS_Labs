#!/bin/bash
# 5) Display the factorial of a given n.

echo "Enter n:"
read n

if [ "$n" -lt 0 ] 2>/dev/null
then
    echo "Factorial is not defined for negative numbers"
    exit 1
fi

fact=1
i=1

while [ $i -le $n ]
do
    fact=`expr $fact \* $i`
    i=`expr $i + 1`
done

echo "Factorial of $n is $fact"
