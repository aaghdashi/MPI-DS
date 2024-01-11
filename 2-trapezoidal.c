#include <stdio.h>
#include "mpi.h"


main(int argc, char** argv) {
    int         my_rank,p;
    float       a = 0.0;   
    float       b = 5;   
    int         n = 1024;

    float       h;       
    float       local_a; 
    float       local_b; 
    int         local_n; 

    float       integral;  
    float       sum=0;     
    int         source;    
    int         dest = 0;  
    int         tag = 0;
    MPI_Status  status;
    double start,end;

    float Trap(float local_a, float local_b, int local_n,float h);    

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    start=MPI_Wtime();
    h = (b-a)/n;    
    local_n = n/p;  

    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    integral = Trap(local_a, local_b, local_n, h);


    if (my_rank == 0) {
        sum = integral;
        for (source = 1; source < p; source++) {
            MPI_Recv(&integral, 1, MPI_FLOAT, source, tag,MPI_COMM_WORLD, &status);
            sum+= integral;
        }
    } else {  
        MPI_Send(&integral, 1, MPI_FLOAT, dest,tag, MPI_COMM_WORLD);
    }
      MPI_Barrier(MPI_COMM_WORLD);
      end=MPI_Wtime();

    if (my_rank == 0) {
        printf("With n = %d , our result\n",n);
        printf("of the integral from %f to %f = %f\n",a, b, sum);
        printf("Elapsed time is:%lf\n",end-start);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();
} 


float Trap(float  local_a,float  local_b,int local_n,float  h) {

    float integral;   
    float x;
    int i;

    float f(float x); 

    integral = (f(local_a) + f(local_b))/2.0;
    x = local_a;
    for (i = 1; i <= local_n-1; i++) {
        x = x + h;
        integral = integral + f(x);
    }
    integral = integral*h;
    return integral;
} 


float f(float x) {
    return x*x;
} 



