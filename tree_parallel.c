#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "dot_product.h"

#define ROOT_PROCESS 0

int main(int argc, char *argv[]) {
    int p;
    int my_rank;
    int n = 0;
    int chunk_size = 0;
    int *a = NULL;
    int *b = NULL;
    int *local_a = NULL;
    int *local_b = NULL;
    int local_dot;
    int ok = 1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (my_rank == ROOT_PROCESS) {
        if (!read_vectors_from_stdin(&a, &b, &n)) {
            ok = 0;
            fprintf(stderr, "Invalid input format. Expected: n, v1, v2\n");
        }

        if (ok && !is_power_of_two(n)) {
            ok = 0;
            fprintf(stderr, "Vector length must be a power of two.\n");
        }

        if (!is_power_of_two(p)) {
            ok = 0;
            fprintf(stderr, "Number of processes must be a power of two.\n");
        }

        if (ok && (n % p != 0)) {
            ok = 0;
            fprintf(stderr, "Vector length must be divisible by process count.\n");
        }

        chunk_size = (ok) ? (n / p) : 0;

        for (int rank = 1; rank < p; rank++) {
            MPI_Send(&ok, 1, MPI_INT, rank, 0, MPI_COMM_WORLD);
            if (ok) {
                MPI_Send(&n, 1, MPI_INT, rank, 0, MPI_COMM_WORLD);
                MPI_Send(&chunk_size, 1, MPI_INT, rank, 0, MPI_COMM_WORLD);
                MPI_Send(a + rank * chunk_size, chunk_size, MPI_INT, rank, 0, MPI_COMM_WORLD);
                MPI_Send(b + rank * chunk_size, chunk_size, MPI_INT, rank, 0, MPI_COMM_WORLD);
            }
        }

        if (!ok) {
            free(a);
            free(b);
            MPI_Finalize();
            return 1;
        }

        local_a = a;
        local_b = b;
    } else {
        MPI_Recv(&ok, 1, MPI_INT, ROOT_PROCESS, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (!ok) {
            MPI_Finalize();
            return 1;
        }

        MPI_Recv(&n, 1, MPI_INT, ROOT_PROCESS, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&chunk_size, 1, MPI_INT, ROOT_PROCESS, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        local_a = (int *)malloc((size_t)chunk_size * sizeof(int));
        local_b = (int *)malloc((size_t)chunk_size * sizeof(int));

        if (local_a == NULL || local_b == NULL) {
            free(local_a);
            free(local_b);
            MPI_Finalize();
            return 1;
        }

        MPI_Recv(local_a, chunk_size, MPI_INT, ROOT_PROCESS, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(local_b, chunk_size, MPI_INT, ROOT_PROCESS, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    local_dot = dotProduct(local_a, local_b, chunk_size);

    for (int step = 1; step < p; step *= 2) {
        if ((my_rank % (2 * step)) == 0) {
            if (my_rank + step < p) {
                int received = 0;
                MPI_Recv(&received, 1, MPI_INT, my_rank + step, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_dot += received;
            }
        } else {
            int destination = my_rank - step;
            MPI_Send(&local_dot, 1, MPI_INT, destination, 2, MPI_COMM_WORLD);
            break;
        }
    }

    if (my_rank == ROOT_PROCESS) {
        int golden = dotProduct(a, b, n);
        printf("dot_product_parallel_tree = %d\n", local_dot);
        printf("dot_product_sequential_ref = %d\n", golden);
        printf("check = %s\n", (local_dot == golden) ? "OK" : "FAIL");
    }

    if (my_rank == ROOT_PROCESS) {
        free(a);
        free(b);
    } else {
        free(local_a);
        free(local_b);
    }

    MPI_Finalize();
    return 0;
}