#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 4  // Tamanho das matrizes (para este exemplo)

int main(int argc, char *argv[]) {
    int rank, size, i, j, k;
    int a[N][N], b[N][N], c[N][N];
    int local_n, offset;
    int *sendcounts, *displs;
    int *local_a;
    int *local_c;

    // Inicializar MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Calcular quantas linhas cada processo terá
    local_n = N / size;
    if (N % size != 0) {
        if (rank == 0) printf("O número de processos deve dividir N uniformemente para este exemplo simples.\n");
        MPI_Finalize();
        return 1;
    }

    // Alocar memória para as matrizes locais
    local_a = (int*)malloc(local_n * N * sizeof(int));
    local_c = (int*)malloc(local_n * N * sizeof(int));

    // Alocar arrays para SCATTER e GATHER
    sendcounts = (int*)malloc(size * sizeof(int));
    displs = (int*)malloc(size * sizeof(int));

    // Configurar sendcounts e displs para SCATTER e GATHER
    for (i = 0; i < size; i++) {
        sendcounts[i] = local_n * N;
        displs[i] = i * local_n * N;
    }

    // No processo 0, inicializar as matrizes A e B
    if (rank == 0) {
        printf("Inicializando matrizes A e B no processo 0...\n");
        
        // Inicializar matriz A com valores de exemplo
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                a[i][j] = i + j;
            }
        }
        
        // Imprimir matriz A
        printf("Matriz A:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%d ", a[i][j]);
            }
            printf("\n");
        }
    }
    
    // Inicializar matriz B em todos os processos
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            b[i][j] = i * j + 1;
        }
    }
    
    if (rank == 0) {
        // Imprimir matriz B
        printf("Matriz B (disponível em todos os processos):\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%d ", b[i][j]);
            }
            printf("\n");
        }
    }

    // Distribuir as linhas da matriz A entre os processos usando SCATTER
    MPI_Scatterv(&a[0][0], sendcounts, displs, MPI_INT, local_a, local_n * N, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Cada processo calcula sua parte da matriz C
    for (i = 0; i < local_n; i++) {
        for (j = 0; j < N; j++) {
            local_c[i * N + j] = 0;
            for (k = 0; k < N; k++) {
                local_c[i * N + j] += local_a[i * N + k] * b[k][j];
            }
        }
    }
    
    // Reunir os resultados em matriz C no processo 0 usando GATHER
    MPI_Gatherv(local_c, local_n * N, MPI_INT, &c[0][0], sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Imprimir a matriz C resultante (apenas no processo 0)
    if (rank == 0) {
        printf("Matriz C resultante:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%d ", c[i][j]);
            }
            printf("\n");
        }
    }
    
    // Liberar memória alocada
    free(local_a);
    free(local_c);
    free(sendcounts);
    free(displs);
    
    MPI_Finalize();
    return 0;
}