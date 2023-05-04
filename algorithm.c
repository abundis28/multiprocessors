//Functions with camel, variables with underscore

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emmintrin.h>
#include <immintrin.h>

void CreateTable(){
    printf ("| Run  || Serial | Parallel 1 | Parallel 2 | \n");  
    for (int i = 0; i < 5; i++)  {  
        printf ("| %d || %d | %d | %d | \n ", i, i, i, i); //Cambiar a datos guardados   
    }  
    printf ("| Average  || %d | %d | %d | \n"); //Cambiar a datos guardados
    printf ("| % vs Serial  || -- | %d | %d | \n"); //Cambiar a datos guardados
}


int main(){


    return 0;
}