#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size, i, provided;

    // Initialize MPI
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    // Get how many MPI processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get the rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Hello World from rank %d from %d processes!\n", rank, size);

    // Finalize MPI
    MPI_Finalize();
}