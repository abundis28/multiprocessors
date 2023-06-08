#! /bin/bash

# Copy files of the desired size to the execute folder.
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

# Execute algorithm with the desired matrix.
export PATH=/usr/local/gcc9.3/bin:$PATH
gcc algorithm.c -fopenmp -o algorithm -std=c11 -O -ftree-vectorize -mavx512f  && ./algorithm

# Remove copied files and compiled binary
rm matrizA.txt
rm matrizB.txt
rm algorithm