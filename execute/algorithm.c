//FOR LARGE MATRIX 1024x1024  // 512x2048 // 256x4096
// gcc algorithm.c -o algorithm -std=c99 && ./algorithm 
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <omp.h>
#include <math.h>

// CONSTANTS
static const int MAX_LINE_LENGTH = 1000;

// GLOBAL VARIABLES
const char *fileA = "matrizA.txt";
const char *fileB = "matrizB.txt";
FILE *textfile_a;
FILE *textfile_b;

int row_a, col_a, row_b, col_b, row_bt, col_bt, elements_count;
double* A = NULL;
double* Bt = NULL;
double* B = NULL;
double* C = NULL;
double* C_open = NULL;

double* A_ali = NULL;
double* Bt_ali = NULL;
double* B_ali = NULL;
double* C_ali = NULL;

struct timespec start;
struct timespec finish;

double elapsed;
double original[5], auto_vec[5], open[5];
double average_original, average_auto_vec, average_open;

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

    printf("\nSuccessful load of matrix %c\n\n", id_matrix);
    return 1;
}

int AllocInitMemory() {
    // Alloc memory the normal way
    A = (double*)malloc(sizeof(double) * row_a * col_a);
    Bt= (double*)malloc(sizeof(double) * row_bt * col_bt);
    B = (double*)malloc(sizeof(double) * row_b * col_b);
    C = (double*)malloc(sizeof(double) * row_a * col_b);
    C_open = (double*)malloc(sizeof(double) * row_a * col_b);

    // Alloc aligned memory 
    A_ali =     (double*)aligned_alloc(64, sizeof(double) * row_a * col_a);
    Bt_ali =    (double*)aligned_alloc(64, sizeof(double) * row_bt * col_bt);
    B_ali =     (double*)aligned_alloc(64, sizeof(double) * row_b * col_b);
    C_ali =     (double*)aligned_alloc(64, sizeof(double) * row_a * col_b);

    if((A == NULL) || (Bt == NULL) || (B == NULL) || (C == NULL) /*|| (C_ali == NULL)*/ || (C_open == NULL)){
        return 0;
    }
    printf("Enough memory allocated.\n");
    return 1;
}

int CreateMatrix(char id_matrix) {
    double fp = 0;
    if (id_matrix == 'A') {
        textfile_a = fopen(fileA, "r");
        // printf("File A opened\n");
        for (int i = 0; i < elements_count; i++) {
            fscanf(textfile_a, "%lf\n", &fp);
            // printf("%lf\n", fp);
            A[i] = (double)fp;
            A_ali[i] = (double)fp;
        }
        fclose(textfile_a);
    }
    else if (id_matrix == 'B') {
        textfile_b = fopen(fileB, "r");
        // printf("File B opened\n");
        for (int i = 0; i < elements_count; i++) {
            fscanf(textfile_b, "%lf\n", &fp);
            B[i] = (double)fp;
            B_ali[i] = (double)fp;
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
            Bt_ali[(j * row_b) + i] = B_ali[(i * col_b) + j];
        }
    }
    printf("\nSuccessful transposing of Matrix B\n");
    return 1;
}

int CreateTable() {
    average_open /= 5;
    average_original /= 5;
    average_auto_vec /= 5;
    printf ("\n|  Run ||    Serial\t|  AutoVec\t|  OpenMP\t|\n");  
    printf("---------------------------------------------------------\n"); 
    
    for (int i = 0; i < 5; i++)  {  
        printf ("|   %d  || %7.10lf |  %7.10lf  | %7.10lf |\n", i+1, original[i], auto_vec[i], open[i]); //Cambiar a datos guardados   
    } 

    printf("---------------------------------------------------------\n"); 
    printf ("|  Avg || %7.10lf |  %7.10lf  | %7.10lf |\n", average_original, average_auto_vec, average_open); //Cambiar a datos guardados
    printf ("| %%Eff ||\t---\t|    %7.4lf\t| %7.4lf\t| \n\n", average_auto_vec/average_original, average_open/average_original); //Cambiar a datos guardados
    
    return 1;
}

void FastestMethod(){
    if((average_original > average_open) && (average_auto_vec > average_open)){
        printf("OpenMP is the fastest method.\n\n");
    }else if((average_original > average_auto_vec) && (average_open > average_auto_vec)){
        printf("Auto-Vectorization is the fastest method.\n\n");
    }else{
        printf("Sequential is the fastest method.\n\n");
    }
}

double MultiplyMatSeq() {
    clock_gettime(CLOCK_REALTIME, &start);

    for(int i = 0; i < row_a ; i++){
        for(int j = 0; j < col_b; j++){
            double sum = 0;
                for(int k = 0; k < row_b; k++){    
                    __asm("nop");          
                    sum += (A[i * col_a + k] * Bt[j * col_a + k]);
                }
            C[i * row_a + j] = sum;
        }
    }
    clock_gettime(CLOCK_REALTIME, &finish);
    elapsed =  (finish.tv_sec - start.tv_sec) + (double)(finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    return elapsed;
}

double MultiplyMatOpenMP() {
    clock_gettime(CLOCK_REALTIME, &start);
    int i, j, k;
    double sum = 0.0;

    #pragma omp parallel shared(A, Bt, C_open) private(i, j ,k) num_threads(8)
    {
        #pragma omp for reduction (+:sum) schedule(static)
        for (i = 0; i < row_a; i++) {
            for (j = 0; j < col_b; j++) {
                sum = 0.0;
                for (k = 0; k < row_b; k++) {
                    __asm("nop");
                    sum += A[i * col_a + k] * Bt[j * col_a + k];
                }
                C_open[i * col_b + j] = sum;
            }
        }
    }
    clock_gettime(CLOCK_REALTIME, &finish);
    elapsed =  (finish.tv_sec - start.tv_sec) + (double)(finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    return elapsed;
}

double MultiplyMatVec() {
    clock_gettime(CLOCK_REALTIME, &start);

    for(int i = 0, row_a_local = row_a; i < row_a_local ; i++){
        for(int j = 0, col_b_local = col_b; j < col_b_local; j++){
            double sum = 0;
            for(int k = 0, row_b_local = row_b; k < row_b_local; k++){              
                sum += (A_ali[i * col_a + k] * Bt_ali[j * col_a + k]);
            }
            C_ali[i * row_a + j] = sum;
        }
    }
    clock_gettime(CLOCK_REALTIME, &finish);
    elapsed =  (finish.tv_sec - start.tv_sec) + (double)(finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    return elapsed;
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
    FILE *f = fopen("matrizC.txt", "ab+");
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
    free(C_ali);
    free(C_open);

    return 1;
}

int CheckResults(char id_matrix){
    double* C_check = NULL;
    if (id_matrix == 'A') {
        C_check = C_ali;
    }
    else if (id_matrix == 'O') {
        C_check = C_open;
    } else{
        return 0;
    }

    for(int i = 0; i < row_a ; i++){
        for(int j = 0; j < col_b; j++){
            if(fabs(C[i * row_a + j] - C_check[i * row_a + j]) > 0.0000000001){
                if(id_matrix == 'A'){
                    printf("Wrong auto-vectorization matrix mutplication result.\n\n");
                }else if(id_matrix == 'O'){
                     printf("Wrong OpenMP matrix multiplication result.\n\n");
                }
                return 0;
            }
        }
    }

    if ('O') {
        printf("\nThe matrix result of OpenMP multiplication is correct.\n");
    } else if ('A') {
        printf("\nThe matrix result of AutoVec multiplication is correc.\n");
    }
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
        FreeAllocatedMemory();
        exit( EXIT_FAILURE );
    }

    if (!TransposeMatrixB()) {
        printf("Error while transposing second matrix.\n\n");
        FreeAllocatedMemory();
        exit( EXIT_FAILURE );
    }

    // Multiply 5 times the matrixes with 3 different methods
    for(int i = 0; i < 5; i++){
        original[i] = MultiplyMatSeq();
        open[i] = MultiplyMatOpenMP();
        auto_vec[i] = MultiplyMatVec();
        average_original += original[i];
        average_open += open[i];
        average_auto_vec += auto_vec[i];
    }
    printf("\nSuccessful multiplication of the matrixes\n");

    if (!WriteResultMatrixToTxt()) {
        printf("Error while writing result matrix to txt file.\n\n");
        FreeAllocatedMemory();
        exit( EXIT_FAILURE );
    }
    
    if (!CheckResults('O')) {
        FreeAllocatedMemory();
        exit( EXIT_FAILURE );
    }

    if (!CreateTable()) {
        printf("Error while printing table with runtime comparison.\n\n");
        FreeAllocatedMemory();
        exit( EXIT_FAILURE );
    }

    FastestMethod(); 

    if (!FreeAllocatedMemory()) {   
        printf("Error while freeing allocated memory.\n\n");
        exit( EXIT_FAILURE );
    }
    
    return 0;
}