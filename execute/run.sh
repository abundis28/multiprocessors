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
elif [ $1 == "p" ]
then
    cp ../matrix/matrix10000/* .
fi

export PATH=/usr/local/gcc9.3/bin:$PATH
gcc algorithm.c -fopenmp -o algorithm -std=c11 -O -ftree-vectorize -mavx512f  && ./algorithm

rm matrizA.txt
rm matrizB.txt
rm algorithm