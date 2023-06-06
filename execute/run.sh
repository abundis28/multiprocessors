#! /bin/bash

if [ $1 == "s" ]
then
    cp ../matrix/matrix8/* .
elif [ $1 == "m" ]
then
    cp ../matrix/matrix2500/* .
elif [ $1 == "l" ]
then
    cp ../matrix/matrix1048576/* .
fi

gcc algorithm.c -o algorithm -std=c11 -fopenmp && ./algorithm

rm matrix*
rm algorithm