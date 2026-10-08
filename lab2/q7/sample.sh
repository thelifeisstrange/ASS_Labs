#!/bin/bash
# Extra 2) Quadratic equation roots using case on the discriminant.

echo "Enter coefficients a, b, c for ax^2 + bx + c = 0:"
read a b c

if [ -z "$a" ] || [ "$a" = "0" ]
then
    echo "a must be a non-zero number"
    exit 1
fi

d=`echo "$b * $b - 4 * $a * $c" | bc`

if [ `echo "$d > 0" | bc` -eq 1 ]
then
    kind=positive
elif [ `echo "$d == 0" | bc` -eq 1 ]
then
    kind=zero
else
    kind=negative
fi

case $kind in
positive)
    sqrt_d=`echo "sqrt($d)" | bc -l`
    r1=`echo "scale=4; (-($b) + $sqrt_d) / (2 * $a)" | bc -l`
    r2=`echo "scale=4; (-($b) - $sqrt_d) / (2 * $a)" | bc -l`
    echo "Discriminant D = $d > 0 : two distinct real roots"
    echo "Root 1 = $r1"
    echo "Root 2 = $r2"
    ;;
zero)
    r=`echo "scale=4; -($b) / (2 * $a)" | bc -l`
    echo "Discriminant D = $d = 0 : one real root (repeated)"
    echo "Root = $r"
    ;;
negative)
    sqrt_d=`echo "sqrt(-($d))" | bc -l`
    real=`echo "scale=4; -($b) / (2 * $a)" | bc -l`
    imag=`echo "scale=4; $sqrt_d / (2 * $a)" | bc -l`
    echo "Discriminant D = $d < 0 : two complex roots"
    echo "Root 1 = $real + ${imag}i"
    echo "Root 2 = $real - ${imag}i"
    ;;
esac
