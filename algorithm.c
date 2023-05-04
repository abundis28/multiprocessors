//Functions with camel, variables with underscore

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emmintrin.h>
#include <immintrin.h>

// CONSTANTS
#define MAX_LINE_LENGTH 1000

int row_a, column_a, row_b, column_b;


int MatrixValidation(char id_matrix, int elements_count){
    int row = 0, column = 0;
    do{
        printf("\nPlease write the number of rows matrix %c : ", id_matrix);
        scanf("%d", &row);
        printf("Please write the number of columns matrix %c : ", id_matrix);
        scanf("%d", &column);
        if(id_matrix == 'A'){
            if((row *column) <= elements_count){
              return row, column;  
            }
            else{
                printf("\n ERROR: Please enter dimensions that not overpass %d.\n ", elements_count);
            }
        }
        else if(id_matrix == 'B'){
            if(((row *column) <= elements_count) && (column_a == row)){
                return row, column;
            }
            else if ((row *column) <= elements_count){
                printf("\n ERROR: Wrong dimensions. Please take into account that the rows of matrix %c must match %d. \n", id_matrix, column_a);
            }
            else{
                printf("\n ERROR: Size too big, the dimensions must not overpass %d elements. Please take into account that the rows of matrix %c must match %d. \n", elements_count, id_matrix, column_a);
            }
        }
    }while (1 == 1);

    
}

// FUNCTIONS
int OpenFile(char id_matrix) {
    printf("Opening file: ");
    FILE    *textfile;
    char    line[MAX_LINE_LENGTH], ch;
    int     elements_count;

    if (id_matrix == 'A'){
        printf("A\n");
        textfile = fopen("matrix/matrixA2500.txt", "r");
        elements_count = 0;
        for (ch = getc(textfile); ch != EOF; ch = getc(textfile)){
            if (ch == '\n') {
                elements_count++;
            }
        }
        row_a, column_a = MatrixValidation(id_matrix, elements_count);

    } else if (id_matrix == 'B') {
        printf("B\n");
        textfile = fopen("matrix/matrixB2500.txt", "r");
        elements_count = 0;
        for (ch = getc(textfile); ch != EOF; ch = getc(textfile)){
            if (ch == '\n') {
                elements_count++;
            }
        }
        row_b, column_b = MatrixValidation(id_matrix, elements_count);
    }
    
    if(textfile == NULL) {
        printf("Null file for matrix %c, try again!\n", id_matrix);
        return 0;
    }
    
    fclose(textfile);
    printf("Successful load of matrix %c\n", id_matrix);
    return 1;
}

int LoadMatrixes() {
    if(!OpenFile('A') || !OpenFile('B')) {
        printf("Error while loading matrixes.");
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

int main(){
    int res = LoadMatrixes();
    return 0;
}