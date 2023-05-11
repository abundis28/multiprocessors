A = dlmread('matrixA2500.txt');
A = reshape(A,25, 100);
%A = A.';
B = dlmread('matrixB2500.txt');
B = reshape(B, 100, 25);
%B = B.';
C = A*B;