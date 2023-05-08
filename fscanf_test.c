#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE    *textfile;
    textfile = fopen("matrix/matrixA2500.txt", "r");
    double fp;
    for (int i = 0; i < 2500; i++) {
        fscanf(textfile, "%f", &fp);
        printf("%f - %d\n", fp, sizeof(fp));
    }
    return 0;
}