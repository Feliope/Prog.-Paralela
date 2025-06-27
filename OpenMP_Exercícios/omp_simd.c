#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int main(int argc, char **argv) {

    long int N = atol(argv[1]); //Long int e o atol para suportar números maiores
    //Utilizei N = 100.000.000 pois foi o que minha máquina suportava
    long int i;
    double dot = 0.0;

    int* a = (int*) malloc(sizeof(int) * N);
    int* b = (int*) malloc(sizeof(int) * N);

    for (i = 0; i < N; i++) {
        a[i] = 2;
        b[i] = 5;
    }

    clock_t start, end;
    double time_taken;

    printf("========== Simd ==========\n\n");
    
    start = clock();
    
    #pragma omp simd
    for(i = 0; i < N; i++) {
        dot += a[i] * b[i];
    }
    
    end = clock();
    time_taken = (double)(end - start) / (double)CLOCKS_PER_SEC;
    printf("Tempo de execução: %f segundos\n\n", time_taken);

    free(a);
    free(b);

    return 0;
}