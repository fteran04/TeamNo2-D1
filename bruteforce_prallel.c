#include <stdio.h>
#include <mpi.h>

#define ROOT_PROCESS 0

void printVec(int arr[], int n){
    printf("< ");
    for (int i = 0; i < n; i++){
        printf("%d, ", arr[i]);
    }
    printf(">");
    printf("\n");
}

int main ( int argc, char *argv[] ){

    int p, my_rank, i, ans = 0, vecPart;

    int a[] = {4, 7, 2, 1, 9, 12, 5, 6};
    int b[] = {10, 5, 9, 4, 2, 7, 2, 8};
    int n = 8;

    

    MPI_Init ( &argc, &argv );
    MPI_Comm_rank ( MPI_COMM_WORLD, &my_rank );
    MPI_Comm_size ( MPI_COMM_WORLD, &p );

    if(my_rank == ROOT_PROCESS){
        
        printf("a = ");
        printVec(a, n);

        printf("b = ");
        printVec(b, n);

        printf("\n");

        for ( int rank = 1; rank < p; rank++ )
        MPI_Send ( &n, 1, MPI_INT, rank, 0, MPI_COMM_WORLD );
    }

    else{
        MPI_Recv ( &n, 1, MPI_INT, ROOT_PROCESS, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
    }

    int part = 0;

    for ( i = my_rank; i < n; i += p )
    {
      part += a[i] * b[i];
    }

    if ( my_rank == ROOT_PROCESS )
    {
      /* consolidate the contributions */
      ans = 0;
      ans += part;
      for ( int rank = 1; rank < p; rank++ )
      {
        MPI_Recv ( &vecPart, 1, MPI_INT, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
        ans += vecPart;
      }

      printf("a • b = %d\n", ans);

    }
    else
    {
      /* send contribution */
      MPI_Send ( &part, 1, MPI_INT, ROOT_PROCESS, 0, MPI_COMM_WORLD );
    }


    MPI_Finalize ();
    return 0;
}