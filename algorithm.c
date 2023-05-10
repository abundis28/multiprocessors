//Functions with camel, variables with underscore

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emmintrin.h>
#include <immintrin.h>

// CONSTANTS
#define MAX_LINE_LENGTH 1000

// GLOBAL VARIABLES
int row_a, col_a, row_b, col_b, elements_count;
double* A = NULL;
double* B = NULL;
double* C = NULL;
FILE *textfile_a;
FILE *textfile_b;

// FUNCTIONS
int MatrixValidation(char id_matrix, FILE *textfile){
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
        textfile_a = fopen("matrix/matrixA2500.txt", "r");
        if(textfile_a == NULL) {
            printf("Null file for matrix %c, try again!\n", id_matrix);
            return 0;
        }
        MatrixValidation(id_matrix, textfile_a);
        printf("Dimensions %d, %d are valid for matrix A\n", row_a, col_a);
    } else if (id_matrix == 'B') {
        textfile_b = fopen("matrix/matrixB2500.txt", "r");
        if(textfile_b == NULL) {
            printf("Null file for matrix %c, try again!\n", id_matrix);
            return 0;
        }
        MatrixValidation(id_matrix, textfile_b);
        printf("Dimensions %d, %d are valid for matrix B\n", row_b, col_b);
    }

    printf("Successful load of matrix %c\n", id_matrix);
    return 1;
}

void alloc_init_mem(int i){
    // Alloc memory according to instruction set used
    A = (double*)malloc(sizeof(double) * row_a * col_a);
    B = (double*)malloc(sizeof(double) * row_b * col_b);
    C = (double*)malloc(sizeof(double) * row_a * col_b);
    printf("\nMemory allocated\n");
}

int CreateMatrix(char id_matrix) {
    printf("\nCREATING MATRIX\n");
    double fp = 0;
    if (id_matrix == 'A') {
        printf("%d\n", elements_count);
        for (int i = 0; i < elements_count; i++) {
            fscanf(textfile_a, "%lf\n", &fp);
            printf("i: %d - val: %lf\n", i, (double)fp);
            A[i] = (double)fp;
        }
        printf("\nMATRIX A\n");
    }
    else if (id_matrix == 'B') {
        printf("%d\n", elements_count);
        for (int i = 0; i < elements_count; i++) {
            fscanf(textfile_b, "%lf\n", &fp);
            // printf("i: %d - val: %lf\n", i, (double)fp);
            B[i] = (double)fp;
        }
        printf("\nMATRIX B\n");
    }
    return 1;
}

int LoadMatrixes() {
    if(!OpenFile('A') || !OpenFile('B')) {
        printf("Error while loading matrixes.");
        return 0;
    }
    alloc_init_mem(0);
    if(!CreateMatrix('A') || !CreateMatrix('B')) {
        printf("Error while creating matrixes.");
        return 0;
    }
    return 1;
}

void CreateTable(){
    printf ("| Run  || Serial | Parallel 1 | Parallel 2 | \n");  
    for (int i = 0; i < 5; i++)  {  
        printf ("| %d || %d | %d | %d | \n ", i, i, i, i); //Cambiar a datos guardados   
    }  
    printf ("| Average  || %d | %d | %d | \n"); //Cambiar a datos guardados
    printf ("| % vs Serial  || -- | %d | %d | \n"); //Cambiar a datos guardados
}

void print_matrix() {
    for (int r = 0; r < row_a; r++) {
        for (int c = 0; c < col_a; c++) {
            if( A )
                printf("%lf ", *(A + r*row_a + c));
        }
        printf("\n");
    }
    for (int r = 0; r < row_b; r++) {
        for (int c = 0; c < col_b; c++) {
            if( B )
                printf("%lf ", *(A + r*row_b + c));
        }
        printf("\n");
    }
}

// MAIN
int main(){
    int res = LoadMatrixes();

    fclose(textfile_a);
    fclose(textfile_b);

    free(A);
	free(B);
	free(C);
    return 0;
}