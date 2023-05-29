formatSpec = '%f';

A_ID = fopen('matrixA2500.txt','r');
A = fscanf(A_ID,formatSpec);
A = reshape(A,25, 100);
A = A.';

B_ID = fopen('matrixB2500.txt','r');
B = fscanf(B_ID,formatSpec)
B = reshape(B, 100, 25);
B = B.';
C = A*B;
C2 = C.';
C2 = C2(:);

writematrix(C2,'C_Val.txt');