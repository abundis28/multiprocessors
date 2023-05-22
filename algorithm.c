//Functions with camel, variables with underscore
//FOR LARGE MATRIX 1024x1024  // 512x2048 // 256x4096
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emmintrin.h>
#include <immintrin.h>

// CONSTANTS
static const int MAX_LINE_LENGTH = 1000;
const char *fileA = "matrix/matrixA8.txt";
const char *fileB = "matrix/matrixB8.txt";

// GLOBAL VARIABLES
int row_a, col_a, row_b, col_b, elements_count;
double* A = NULL;
double* Bt = NULL;
double* B = NULL;
double* C = NULL;
FILE *textfile_a;
FILE *textfile_b;

clock_t start, finish;
double elapsed;

//TIMESTAMP VARIABLES
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
    printf("\nOpening file: ");

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

    printf("Successful load of matrix %c\n", id_matrix);
    return 1;
}

void AllocInitMemory(int i) {
    // Alloc memory according to instruction set used
    A = (double*)malloc(sizeof(double) * row_a * col_a);
    Bt= (double*)malloc(sizeof(double) * row_b * col_b);
    B = (double*)malloc(sizeof(double) * row_b * col_b);
    C = (double*)malloc(sizeof(double) * row_a * col_b);
    printf("\nMemory allocated\n");
}

int CreateMatrix(char id_matrix) {
    double fp = 0;
    if (id_matrix == 'A') {
        textfile_a = fopen(fileA, "r");
        printf("\nCreating matrix A\n");
        for (int i = 0; i < elements_count; i++) {
            fscanf(textfile_a, "%lf\n", &fp);
            A[i] = (double)fp;
        }
        fclose(textfile_a);
        printf("\nSuccessful creation of matrix %c\n", id_matrix);
    }
    else if (id_matrix == 'B') {
        textfile_b = fopen(fileB, "r");
        printf("\nCreating matrix B\n");
        for (int i = 0; i < elements_count; i++) {
            fscanf(textfile_b, "%lf\n", &fp);
            B[i] = (double)fp;
        }
        fclose(textfile_b);
        printf("\nSuccessful creation of matrix %c\n", id_matrix);
    }
    return 1;
}

void TransposeMatrixB() {
    printf("Transposing Matrix");
    for (int i = 0; i < row_b; ++i) {
        for (int j = 0; j < col_b; ++j) {
            Bt[(j * row_b) + i] = B[(i * col_b) + j];
        }
    }
    printf("Successful Transposing Matrix");
}

void CreateTable() {
    printf ("| Run  ||       Serial       | Parallel 1 | Parallel 2 |\n");  
    printf("----------------------------------------------------------\n"); 
    
    for (int i = 0; i < 5; i++)  {  
        printf ("|   %d  || %7.10lf | %7.10lf | %7.10lf |\n", i+1, original[i], posix[i], open[i]); //Cambiar a datos guardados   
    } 

    printf("----------------------------------------------------------\n"); 
    printf ("| Avg  || %7.10lf | %7.10lf | %7.10lf |\n", average_original/5, average_posix/5, average_open/5); //Cambiar a datos guardados
    printf ("| %%Eff ||      ---      | %d | %d | \n"); //Cambiar a datos guardados
}

double MultiplyMatrixes() {
    start = clock();
    // printf("\n");
    for(int i=0; i < row_a ; i++){
        for(int j=0; j < col_b; j++){
            double sum =0;
            // printf("Cell: %d - %d:\n", i, j);
            for(int k=0; k<row_b; k++){              
                sum += (A[i * col_a + k] * B[k * col_b + j]);
                // printf("%lf(%d,%d) * %lf(%d,%d) = %lf\n", A[i * col_a + k], i, k, B[k * col_b + j], k, j, sum);
            }
            // printf("\n");
            C[i * row_a + j] = sum;
        }
    }
    finish = clock();
    elapsed = (finish - start);
    printf("\nSuccessful multiplication of the matrixes\n");
    return elapsed;
}

void PrintMatrixes() {
    printf("\n\n");
    for (int i = 0; i < elements_count; i++) {
        printf("%lf \t//\t %lf \t//\t %lf\n", A[i], B[i], Bt[i]);
    }
}

void WriteResultMatrixToTxt() {
    FILE *f = fopen("C.txt", "wb");
    for (int i = 0; i < row_a * col_b; i++) {
        fprintf(f, "%.10lf\n", C[i]);
    }
    fclose(f);
}

// MAIN
int main(){
    if (!OpenFile('A') || !OpenFile('B')) {
        printf("Error while loading the matrixes.");
        exit( EXIT_FAILURE );
    }

    AllocInitMemory(0);

    if (!CreateMatrix('A') || !CreateMatrix('B')) {
        printf("Error while creating the matrixes.");
        exit( EXIT_FAILURE );
    }

    for(int i = 0; i < 5; i++){
        original[i] = MultiplyMatrixes();
        average_original += original[i];
        //printf("\nTime elapsed: %lf\n", original[i]);
    }

    TransposeMatrixB();
    WriteResultMatrixToTxt();
    CreateTable();

    free(A);
	free(B);
	free(C);
    return 0;
}