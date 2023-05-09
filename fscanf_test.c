#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE    *textfile;
    textfile = fopen("matrix/matrixA2500.txt", "r");
    double fp = 0;
    for (int i = 0; i < 2500; i++) {
        fscanf(textfile, "%lf\n", &fp);
        printf("%lf\n", (double)fp);
    }
    return 0;
}