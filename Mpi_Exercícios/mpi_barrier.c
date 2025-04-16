#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]) { /* mpi_barrier.c  */
int my_rank, num_procs;
MPI_Comm comm=MPI_COMM_WORLD;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(comm,&num_procs);
    MPI_Comm_rank(comm,&my_rank);
    
    if (my_rank == 0 ) {
        printf ("Estou atrasado para a barreira! \n");
        getchar();
    	MPI_Barrier(comm);
    }
    else
    	MPI_Barrier(comm);
    printf("Passei da barreira. Eu sou o %d de %d processos \n", my_rank, num_procs);
  
    MPI_Finalize();
    return(0);
}