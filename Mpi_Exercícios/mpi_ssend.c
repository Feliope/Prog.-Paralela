#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[]) { /* mpi_ssend.c  */
int i, meu_ranque, num_procs;
int pot2, destino, meu_valor;
int reducao, recebido, etiq=1, cont=1;
MPI_Status estado;
MPI_Request requisicao;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    /* Aborta se o número de processos não for potência de 2 */
    pot2 = num_procs;
    while (((pot2 % 2) == 0) && pot2 > 1)
        pot2 /= 2;
    if (pot2 != 1) {
        if (meu_ranque == 0)
           printf("Por favor execute com número de processos potencia de 2 \n");
        MPI_Finalize();
        exit(0);
        }
    /* Cada processo tem um valor diferente para a redução */
    meu_valor = meu_ranque*num_procs;
    reducao = meu_valor;
    /* Realiza a troca de mensagens no padrão do algoritmo de "recursive doubling" */
    for (i = 1; i <= (num_procs/2); i += i) {
        if ((meu_ranque/i)%2 == 0) {
            destino = meu_ranque + i;
    /* Metade dos processos envia primeiro e recebe depois */
            MPI_Irecv(&recebido, cont, MPI_INT, destino, etiq, MPI_COMM_WORLD, &requisicao); 
            MPI_Ssend(&reducao, cont, MPI_INT, destino, etiq, MPI_COMM_WORLD);
            
            MPI_Wait( &requisicao, &estado);
    /* Realiza operação de redução com os dados recebidos */
            if (recebido > reducao)
                reducao = recebido;
        }
        else {
            destino = meu_ranque-i;
    /* A outra metade dos processos recebe primeiro e envia depois */
            MPI_Irecv(&recebido, cont, MPI_INT, destino, etiq, MPI_COMM_WORLD, &requisicao);
            MPI_Ssend(&reducao, cont, MPI_INT, destino, etiq, MPI_COMM_WORLD);
            MPI_Wait( &requisicao, &estado);
    /* Realiza operação de redução com os dados recebidos */
            if (recebido > reducao)
                reducao = recebido;
        }
    }
    printf("Meu valor = %d, redução = %d \n", meu_valor, reducao);
    MPI_Finalize();
    return(0);
}