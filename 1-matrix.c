#include <time.h>
#include <stdio.h>
#include "mpi.h"
#define N 64


MPI_Status status;

double a[N][N],b[N][N],c[N][N];

main(int argc, char **argv)
{
  int numtasks,rank,numworkers,source,dest,rows,offset,i,j,k;
  double start, stop;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

 if (numtasks > N) {
		  if (rank == 0) 
			   printf("Error: Comm size must be less than %d\n", N);
		  return -1;
 }
if (rank == numtasks - 1) { 
		  rows += N % numtasks;
	 }


  numworkers = numtasks-1;
start=MPI_Wtime();  
  if (rank == 0) {
    for (i=0; i<N; i++) {
      for (j=0; j<N; j++) {
	a[i][j]=(i+1)*(j+1);
	b[i][j]=(i+1)*(j+1);
      }
    }



    /* send matrix a,b */
    rows = N/numworkers;	
    offset = 0;

    for (dest=1; dest<=numworkers; dest++)
    {
      MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&a[offset][0], rows*N, MPI_DOUBLE,dest,1, MPI_COMM_WORLD);
      MPI_Send(&b, N*N, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
      offset = offset + rows;
    }

  
    for (i=1; i<=numworkers; i++)
    {
      source = i;
      MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&c[offset][0], rows*N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
    }
 


    printf("result matrix:\n");
    for (i=0; i<N; i++) {
      for (j=0; j<N; j++)
        printf("%.0lf\t", c[i][j]);
      printf ("\n");
    }
  }
 if (rank > 0) {
    source = 0;
    MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&a, rows*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&b, N*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
 

    for (k=0; k<N; k++)
      for (i=0; i<rows; i++) {
        c[i][k] = 0.0;
        for (j=0; j<N; j++)	  
          c[i][k] = c[i][k] + a[i][j] * b[j][k];
      }

    MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&c, rows*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
  }

stop=MPI_Wtime();
    printf("Elapsed time: %.6f\n\n",stop-start);
  MPI_Finalize();
}
