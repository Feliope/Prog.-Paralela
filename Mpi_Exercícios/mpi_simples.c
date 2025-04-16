#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char *argv[]) {
int my_rank, num_procs;
int origin, destiny, tag = 0;
char message[200];
MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  if (my_rank != 0){
    sprintf(message, "Process %d is alive!", my_rank);
    destiny = 0;
    MPI_Send(message, strlen(message)+1, MPI_CHAR, destiny, tag, MPI_COMM_WORLD);
  }
  
  else {
    for (origin = 1; origin < num_procs; origin++) {
      MPI_Recv(message, 200, MPI_CHAR, origin, tag, MPI_COMM_WORLD, &status);
      printf("%s\n", message);
    }
  }

  MPI_Finalize();
  return 0;
}