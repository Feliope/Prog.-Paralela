#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[]) { /* mpi_isend.c  */
int i, my_rank, num_procs;
int pow2, destiny, my_value;
int reduction, received, tag=1, count=1;
MPI_Status status;
MPI_Request request_send;
MPI_Request request_receive;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    /* Aborta se número de processos não for potência de 2 */
    pow2 = num_procs;
    while (((pow2 % 2) == 0) && pow2 > 1)
        pow2 /= 2;
    if (pow2 != 1) {
        if (my_rank == 0)
           printf("Please run with number of processes power of 2 \n");
        MPI_Finalize();
        exit(0);
        }
    /* Cada processo tem um valor diferente para a redução */
    my_value = my_rank*num_procs;
    reduction = my_value;
    /* Realiza a troca de mensagens no padrão do algoritmo de "recursive doubling" */
    for (i = 1; i <= (num_procs/2); i += i) {
        if ((my_rank/i)%2 == 0) 
            destiny = my_rank + i;
        else 
            destiny = my_rank-i;
    /* Posta os envios e recepções em qualquer ordem */
        MPI_Ssend(&reduction, count, MPI_INT, destiny, tag, MPI_COMM_WORLD);
        MPI_Irecv(&received, count, MPI_INT, destiny, tag, MPI_COMM_WORLD, &request_receive);
    /* As rotinas de "MPI_Wait" asseguram que os dados já foram transmitidos e receiveds */
        MPI_Wait(&request_send, &status);
        MPI_Wait(&request_receive, &status);
    /* Realiza operação de redução com os dados receiveds */
        if (received > reduction)
            reduction = received;
    }
    printf("My value = %d, reduction = %d \n", my_value, reduction);
    MPI_Finalize();
    return(0);
}