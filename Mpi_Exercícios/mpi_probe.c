#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX 1000

int main(int argc, char *argv[]) { /* mpi_probe.c  */
int my_rank, num_procs, tag=0;
int* vet_num;
int total_num, origin, destiny=1, numbers[MAX];
MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (my_rank != destiny) {
        srand(my_rank);
        total_num = (rand() / (float)RAND_MAX) * MAX;
        MPI_Send(numbers, total_num, MPI_INT, destiny, tag, MPI_COMM_WORLD);
        printf("Process %d sent %d numbers to process %d.\n", my_rank, total_num, destiny);
    } 
    else 
        if (my_rank == destiny) {
            for (origin = 1; origin < num_procs; origin++){
                MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                MPI_Get_count(&status, MPI_INT, &total_num);
                vet_num = (int*) malloc(sizeof(int)*total_num);
                origin = status.MPI_SOURCE;
                tag = status.MPI_TAG;
                MPI_Recv(vet_num, total_num, MPI_INT, origin, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("Processo %d recebeu dinamicamente %d números de %d.\n", destiny, total_num, origin);
                free(vet_num);
            } 
        }
    MPI_Finalize();
    return(0);
}