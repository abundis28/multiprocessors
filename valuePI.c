#include <stdio.h>
#include <time.h>
#include <omp.h>
#define MAXTHREADS 1
long cantidadIntervalos = 100000000; //100 Million
double baseIntervalo;
double acum = 0; //Can¥t be a shared variable
clock_t start, end;

void main() {
   int THREADS = MAXTHREADS;
   baseIntervalo = 1.0 / (double)cantidadIntervalos;

   double totalSum = 0;
   omp_set_num_threads(THREADS);
   start = clock();
   #pragma omp parallel
   {
   int numThread = omp_get_thread_num();
   double x, fdx = 0; //Can¥t be a shared variable, must be private to thread.
   for (long i = numThread; i < cantidadIntervalos; i += THREADS) {
      x = i * baseIntervalo;
      fdx = 4 / (1 + x * x);
      acum += fdx;  //acumulate result to global counter
      }
   }
   end = clock();
   acum *= baseIntervalo; //Multiply accumulated heights of the rectangles by the base size.
   printf("\nResultado (%d threads) = %20.18lf (%ld)\n", THREADS, acum, end - start);
}
