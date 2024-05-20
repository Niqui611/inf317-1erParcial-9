#include <stdio.h>
#include <mpi.h>

#define N 20 // Número total de términos en la serie
#define M 2  // Número de procesadores (vectores)

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int terms_per_process = N / size; // Términos por proceso
    int start_term = rank * terms_per_process + 1; // Término inicial para este proceso
    int end_term = start_term + terms_per_process - 1; // Término final para este proceso
    if (rank == size - 1) {
        end_term = N; 
    }

    int vector[M][N/M]; 
    int i, j;

    for (i = 0; i < M; i++) {
        for (j = 0; j < N/M; j++) {
            vector[i][j] = 0;
        }
    }

    for (i = start_term; i <= end_term; i++) {
        for (j = 0; j < M; j++) {
            vector[j][(i - start_term)] = i * 2; 
        }
    }

    int all_vectors[M][N/M * size];
    MPI_Allgather(vector, M * terms_per_process, MPI_INT, all_vectors, M * terms_per_process, MPI_INT, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Términos en los vectores:\n");
        for (i = 0; i < M; i++) {
            printf("Vector %d: ", i);
            for (j = 0; j < N/M * size; j++) {
                printf("%d ", all_vectors[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}
