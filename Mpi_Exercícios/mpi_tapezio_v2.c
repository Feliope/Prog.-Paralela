#include <stdio.h>
#include <math.h>
#include "mpi.h"

double f(double x) {
        double return_val;
        return_val = x*x;
        return return_val;
}

int main(int argc, char *argv[]) {
    int my_rank, num_procs;
    double a = 0.0,  b = 1.0;   
    double initial_time, final_time;
    long int n = 500000000;     
    double h, local_a, local_b;
    double integral=0.0, total;
    long int local_n;
    int tag = 3;
    int destiny = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    h = (b - a)/n;   

    // Dividindo o intervalo em blocos consecutivos
    local_n = n / num_procs;  // Número de subintervalos para cada processo
    
    // Calculando o intervalo local para cada processo
    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;
    
    // Se for o último processo, ajuste o local_b para garantir que chegue até b
    if (my_rank == num_procs - 1) {
        local_b = b;
    }

    initial_time = MPI_Wtime();
    
    // Aplicando a regra do trapézio no intervalo local
    integral = (f(local_a) + f(local_b)) / 2.0;
    for (double x = local_a + h; x < local_b; x += h) {
        integral += f(x);
    }
    integral = integral * h;
    
    // Reduzindo os resultados para o processo 0
    if (my_rank == 0) {
        total = integral;
        for (int origin = 1; origin < num_procs; origin++) {
            MPI_Recv(&integral, 1, MPI_DOUBLE, origin, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total += integral;
        }
        final_time = MPI_Wtime();
        printf("Foram gastos %3.5f segundos\n", final_time - initial_time);
        printf("Com n = %ld trapezoides, a estimativa\n", n);
        printf("da integral de %lf até %lf = %lf \n", a, b, total);
    } else {
        MPI_Send(&integral, 1, MPI_DOUBLE, destiny, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return(0);
}