//FOR LARGE MATRIX 1024x1024  // 512x2048 // 256x4096
// gcc algorithm.c -o algorithm -std=c99 && ./algorithm 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emmintrin.h>
#include <immintrin.h>

// CONSTANTS
static const int MAX_LINE_LENGTH = 1000;
const char *fileA = "matrixA.txt";
const char *fileB = "matrixB.txt";

// GLOBAL VARIABLES
int row_a, col_a, row_b, col_b, row_bt, col_bt, elements_count;
double* A = NULL;
double* Bt = NULL;
double* B = NULL;
double* C = NULL;
FILE *textfile_a;
FILE *textfile_b;
clock_t start, finish;
double elapsed;
double original[5], posix[5], open[5];
double average_original, average_posix, average_open;

// FUNCTIONS
int MatrixValidation(char id_matrix) {
    FILE *textfile;
    if (id_matrix == 'A') {
        textfile = textfile_a;
    }
    else {
        textfile = textfile_b;
    }

    char ch;
    elements_count = 0;
    for (ch = getc(textfile); ch != EOF; ch = getc(textfile)){
        if (ch == '\n')
            elements_count++;
    }

    int row = 0, column = 0;
    do{
        printf("Please write the number of rows matrix %c : ", id_matrix);
        scanf("%d", &row);
        printf("Please write the number of columns matrix %c : ", id_matrix);
        scanf("%d", &column);
        if(id_matrix == 'A'){
            if((row *column) == elements_count) {
                row_a = row;
                col_a = column;
                row_bt = row;
                col_bt = column;
                break;
            }
            else
                printf("\n ERROR: Please enter dimensions that multiply to %d.\n ", elements_count);
        }
        else if(id_matrix == 'B'){
            if(((row *column) == elements_count) && (col_a == row)) {
                row_b = row;
                col_b = column;
                break;
            }
            else
                printf("\n ERROR: Size does not match, the dimensions must be equal to %d elements. Please take into account that the rows of matrix %c must match %d. \n", elements_count, id_matrix, col_a);
        }
    } while (1);
    return 1;
}

int OpenFile(char id_matrix) {
    if (id_matrix == 'A'){
        textfile_a = fopen(fileA, "r");
        if(textfile_a == NULL) {
            printf("Null file for matrix %c, try again!\n", id_matrix);
            return 0;
        }
        MatrixValidation(id_matrix);
        fclose(textfile_a);
        printf("Dimensions %d, %d are valid for matrix A\n", row_a, col_a);
    } else if (id_matrix == 'B') {
        textfile_b = fopen(fileB, "r");
        if(textfile_b == NULL) {
            printf("Null file for matrix %c, try again!\n", id_matrix);
            return 0;
        }
        MatrixValidation(id_matrix);
        fclose(textfile_b);
        printf("Dimensions %d, %d are valid for matrix B\n", row_b, col_b);
    }

    printf("Successful load of matrix %c\n\n", id_matrix);
    return 1;
}

int AllocInitMemory() {
    // Alloc memory according to instruction set used
    A = (double*)malloc(sizeof(double) * row_a * col_a);
    Bt= (double*)malloc(sizeof(double) * row_bt * col_bt);
    B = (double*)malloc(sizeof(double) * row_b * col_b);
    C = (double*)malloc(sizeof(double) * row_a * col_b);
    printf("Memory allocated\n");

    return 1;
}

int CreateMatrix(char id_matrix) {
    double fp = 0;
    if (id_matrix == 'A') {
        textfile_a = fopen(fileA, "r");
        for (int i = 0; i < elements_count; i++) {
            fscanf(textfile_a, "%lf\n", &fp);
            A[i] = (double)fp;
        }
        fclose(textfile_a);
    }
    else if (id_matrix == 'B') {
        textfile_b = fopen(fileB, "r");
        for (int i = 0; i < elements_count; i++) {
            fscanf(textfile_b, "%lf\n", &fp);
            B[i] = (double)fp;
        }
        fclose(textfile_b);
    }
    printf("\nSuccessful creation of matrix %c\n", id_matrix);
    return 1;
}

int TransposeMatrixB() {
    for (int i = 0; i < row_b; ++i) {
        for (int j = 0; j < col_b; ++j) {
            Bt[(j * row_b) + i] = B[(i * col_b) + j];
        }
    }
    printf("\nSuccessful transposing of Matrix B\n");
    return 1;
}

int CreateTable() {
    printf ("| Run  ||       Serial       | Parallel 1 | Parallel 2 |\n");  
    printf("----------------------------------------------------------\n"); 
    
    for (int i = 0; i < 5; i++)  {  
        printf ("|   %d  || %7.10lf | %7.10lf | %7.10lf |\n", i+1, original[i], posix[i], open[i]); //Cambiar a datos guardados   
    } 

    printf("----------------------------------------------------------\n"); 
    printf ("| Avg  || %7.10lf | %7.10lf | %7.10lf |\n", average_original/5, average_posix/5, average_open/5); //Cambiar a datos guardados
    printf ("| %%Eff ||      ---      | %d | %d | \n"); //Cambiar a datos guardados

    return 1;
}

double MultiplyMatSeq() {
    start = clock();
    for(int i = 0; i < row_a ; i++){
        for(int j = 0; j < col_b; j++){
            double sum = 0;
            for(int k = 0; k < row_b; k++){              
                sum += (A[i * col_a + k] * Bt[j * col_a + k]);
            }
            C[i * row_a + j] = sum;
        }
    }
    finish = clock();
    elapsed = (finish - start);
    return elapsed;
}

double MultiplyMatOpenMP() {
    start = clock();

    #pragma omp parallel 
    {
        int i, j, k;
        #pragma omp for
        for(i = 0; i < row_a ; i++){
            for(j = 0; j < col_b; j++){
                double sum = 0;
                for(k = 0; k < row_b; k++){              
                    sum += (A[i * col_a + k] * Bt[j * col_a + k]);
                }
                C[i * row_a + j] = sum;
            }
        }
    }
    finish = clock();
    elapsed = (finish - start);
    return elapsed;
}

double MultiplyMatVec() {
    return 0.0;
}

void PrintMatrixes() {
    printf("\n\n");
    for (int i = 0; i < elements_count; i++) {
        printf("%lf \t//\t %lf \t//\t %lf\n", A[i], B[i], Bt[i]);
    }
}

void PrintResultMatrix() { 
    printf("\n\n");
    for (int i = 0; i < row_a * row_a; i++) {
        printf("%lf\n", C[i]);
    }
}

int WriteResultMatrixToTxt() {
    FILE *f = fopen("C.txt", "wb");
    for (int i = 0; i < row_a * row_a; i++) {
        fprintf(f, "%.10lf\n", C[i]);
    }
    fclose(f);

    return 1;
}

int FreeAllocatedMemory() {
    free(A);
	free(Bt);
    free(B);
	free(C);

    return 1;
}

// MAIN
int main(){
    if (!OpenFile('A') || !OpenFile('B')) {
        printf("Error while loading the matrixes.\n\n");
        exit( EXIT_FAILURE );
    }

    if (!AllocInitMemory()) {
        printf("Error while allocating memory.\n\n");
        exit( EXIT_FAILURE );
    }

    if (!CreateMatrix('A') || !CreateMatrix('B')) {
        printf("Error while creating the matrixes.\n\n");
        exit( EXIT_FAILURE );
    }

    if (!TransposeMatrixB()) {
        printf("Error while transposing second matrix.\n\n");
        exit( EXIT_FAILURE );
    }

    for(int i = 0; i < 5; i++){
        original[i] = MultiplyMatSeq();
        average_original += original[i];
    }
    printf("\nSuccessful sequential multiplication of the matrixes\n\n");

    if (!WriteResultMatrixToTxt()) {
        printf("Error while writing result matrix to txt file.\n\n");
        exit( EXIT_FAILURE );
    }
    
    if (!CreateTable()) {
        printf("Error while printing table with runtime comparison.\n\n");
        exit( EXIT_FAILURE );
    }

    if (!FreeAllocatedMemory()) {
        printf("Error while freeing allocated memory.\n\n");
        exit( EXIT_FAILURE );
    }
    
    return 0;
}