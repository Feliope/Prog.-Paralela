#include <stdio.h>
#include <omp.h>
#define N 1000000000

// Optei por criar uma função auxiliar para calcular o valor de pi,
// assim evitando repetição de código.
// A função recebe o tipo de schedule e o número do clause como parâmetros.

void pi_calculation(const char* scheduleType, int scheduleClause) {
  double inicio, fim, pi = 0.0f; 
  long i;
  
  printf("\n========== %s ==========", scheduleType);

  inicio = omp_get_wtime();

  switch (scheduleClause) {
    case 1: //Static
      #pragma omp parallel for reduction(+: pi) schedule(static)
        for (i = 0; i < N; i++) {
            double t=(double) ((i+0.5)/N);
            pi += 4.0/(1.0+t*t);
      }
      break;

      case 2: //Static with chunk size 3
      #pragma omp parallel for reduction(+: pi) schedule(static, 3)
        for (i = 0; i < N; i++) {
            double t=(double) ((i+0.5)/N);
            pi += 4.0/(1.0+t*t);
      }
      break;

      case 3: //Dynamic
      #pragma omp parallel for reduction(+: pi) schedule(dynamic)
        for (i = 0; i < N; i++) {
            double t=(double) ((i+0.5)/N);
            pi += 4.0/(1.0+t*t);
      }
      break;

      case 4: //Dynamic with chunk size 3
      #pragma omp parallel for reduction(+: pi) schedule(dynamic, 3)
        for (i = 0; i < N; i++) {
            double t=(double) ((i+0.5)/N);
            pi += 4.0/(1.0+t*t);
      }
      break;

      case 5: //Guided
      #pragma omp parallel for reduction(+: pi) schedule(guided)
        for (i = 0; i < N; i++) {
            double t=(double) ((i+0.5)/N);
            pi += 4.0/(1.0+t*t);
      }
      break;

      case 6: //Guided with chunk size 3
      #pragma omp parallel for reduction(+: pi) schedule(guided, 3)
        for (i = 0; i < N; i++) {
            double t=(double) ((i+0.5)/N);
            pi += 4.0/(1.0+t*t);
      }
      break;

      default:
        printf("Invalid schedule type.\n");
        return;
  }

  fim = omp_get_wtime();
  printf("\nValue of pi: %f\n", pi/N);
  printf("Execution time: %f seconds.\n", fim-inicio);
}

int main(int argc, char *argv[]) { /* omp_calcpi.c  */

  pi_calculation("Static", 1);
  pi_calculation("Static with chunk size 3", 2);
  pi_calculation("Dynamic", 3);
  pi_calculation("Dynamic with chunk size 3", 4);
  pi_calculation("Guided", 5);
  pi_calculation("Guided with chunk size 3", 6);

  return(0);
}