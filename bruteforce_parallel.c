#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "dot_product.h"

#define ROOT_PROCESS 0

/*
  Parallel dot product using MPI with brute-force distribution.
 
  Time Complexity:
  O(n/p + p)
 
  - Each process computes the dot product of a chunk of size n/p,
    which costs O(n/p).
  - The root process gathers p partial results and sums them,
    which costs O(p).
 
  Total parallel time:
  T(n,p) = O(n/p + p)
 
  Space Complexity:
  O(n/p) per process (each process stores only its chunk of the vectors).
 */

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
  int total_dot = 0;
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

  if (my_rank == ROOT_PROCESS) {
    total_dot = local_dot;
    for (int rank = 1; rank < p; rank++) {
      int part = 0;
      MPI_Recv(&part, 1, MPI_INT, rank, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      total_dot += part;
    }

    int golden = dotProduct(a, b, n);
    printf("dot_product_parallel_bruteforce = %d\n", total_dot);
    printf("dot_product_sequential_ref = %d\n", golden);
    printf("check = %s\n", (total_dot == golden) ? "OK" : "FAIL");
  } else {
    MPI_Send(&local_dot, 1, MPI_INT, ROOT_PROCESS, 1, MPI_COMM_WORLD);
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