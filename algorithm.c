//Functions with camel, variables with underscore

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emmintrin.h>
#include <immintrin.h>

// CONSTANTS
#define MAX_LINE_LENGTH 1000

// FUNCTIONS
int OpenFile(char id_matrix) {
    printf("Opening file: ");
    FILE    *textfile;
    char    line[MAX_LINE_LENGTH];
    
    if (id_matrix == 'A'){
        printf("A\n");
        textfile = fopen("matrix/matrixA2500.txt", "r");
    } else if (id_matrix == 'B') {
        printf("B\n");
        textfile = fopen("matrix/matrixB2500.txt", "r");
    }
    
    if(textfile == NULL) {
        printf("Null file for matrix %c, try again!\n", id_matrix);
        return 0;
    }
    
    // while(fgets(line, MAX_LINE_LENGTH, textfile)){
    //     printf(line);
    // }
    
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