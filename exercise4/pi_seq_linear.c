
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
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
    int i;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
    t1 = MPI_Wtime(); 

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(SEED * rank); // Important: Multiply SEED by "rank" when you introduce MPI!

    NUM_ITER_PER_PROCESS = NUM_ITER / size;
    
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

    if (rank != 0) {
        MPI_Send(&mycount, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        int* mycount_arr = (int*)malloc(size * sizeof(int));
        mycount_arr[0] = mycount;
        for (i=1; i<size; i++) {
            MPI_Recv(mycount_arr+i, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Process %d of %d | receive count from process %d: %d\n", rank, size, mycount_arr[i], i);
        }

        for (i=0; i<size; i++) { count += mycount_arr[i]; }

        // Estimate Pi and display the result
        pi = ((double)count / (double)(NUM_ITER_PER_PROCESS*size)) * 4.0;
        printf("Process %d of %d | combined count: %d, estimated pi: %f\n", rank, size, count, pi);
        t2 = MPI_Wtime(); 
        printf("Process %d of %d | Execution time: %11.8f s\n", rank, size, (t2 - t1));
    }
    
    MPI_Finalize();
    return 0;
}