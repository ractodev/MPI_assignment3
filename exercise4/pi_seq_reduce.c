
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <mpi.h>

#define SEED     921
#define NUM_ITER 1000000000

double mysecond();

int main(int argc, char* argv[])
{
    int count = 0;
    int mycount = 0;
    double x, y, z, pi;
    double t1, t2;
    
    int rank, size, provided;
    int iter, NUM_ITER_PER_PROCESS;

    t1 = mysecond();
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(SEED * rank); // Important: Multiply SEED by "rank" when you introduce MPI!

    NUM_ITER_PER_PROCESS = NUM_ITER / size;
    MPI_Bcast(&NUM_ITER_PER_PROCESS, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Calculate PI following a Monte Carlo method
    for (iter = 0; iter < NUM_ITER_PER_PROCESS; iter++)
    {
        // Generate random (X,Y) points
        x = (double)random() / (double)RAND_MAX;
        y = (double)random() / (double)RAND_MAX;
        z = sqrt((x*x) + (y*y));
        
        // Check if point is in unit circle
        if (z <= 1.0)
        {
            mycount++;
        }
    }
    printf("Process %d of %d | count: %d\n", rank, size, mycount);
    
    MPI_Reduce(&mycount, &count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        // Estimate Pi and display the result
        pi = ((double)count / (double)(NUM_ITER_PER_PROCESS*size)) * 4.0;
        printf("Process %d of %d | combined count: %d, estimated pi: %f\n", rank, size, count, pi);
        t2 = mysecond();
        printf("Process %d of %d | Execution time: %11.8f s\n", rank, size, (t2 - t1));
    }
    
    MPI_Finalize();
    return 0;
}

// function with timer
double mysecond(){
  struct timeval tp;
  struct timezone tzp;
  int i;

  i = gettimeofday(&tp,&tzp);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}