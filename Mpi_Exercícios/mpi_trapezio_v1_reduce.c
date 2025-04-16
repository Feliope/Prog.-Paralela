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
double x, h;                
double integral=0.0, total;
int origin, destiny = 0;
int tag = 3;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    h = (b - a)/n;   

    initial_time = MPI_Wtime(); 
    if (my_rank == 0) {
        integral = (f(a) + f(b))/2.0;
    }

    for (x = a+h*(my_rank+1); x < b ; x += num_procs*h) {
         integral += f(x);
    }
    integral = integral*h;

    MPI_Reduce(&integral, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        final_time = MPI_Wtime();
        printf("Foram gastos %3.5f segundos\n",final_time-initial_time);
        printf("Com n = %ld trapezoides, a estimativa\n", n);
        printf("da integral de %lf até %lf = %lf \n", a, b, total);
    }
    MPI_Finalize();
    return(0);
} 