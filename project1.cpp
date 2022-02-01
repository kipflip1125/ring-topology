#include "mpi.h"
#include <iostream>
#include <random>
#include <time.h>
#include <string>

int main(int argc, char* argv[]) 
{
    int rank,size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);
    MPI_Comm_size( MPI_COMM_WORLD, &size);

    // first generate random number for each process
    srand(time(NULL) + rank);

    int token = rand() % 90 + 10;
    token = std::stoi("1" + std::to_string(token) + std::to_string(rank));
    int arr[2] = {0,0}; // This is for the highest odd and even values to send arr[0] is even arr[1] is odd
    
    if(rank != 0)
    {
        MPI_Recv(&arr, 2, MPI_INT,(rank - 1) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if(token % 2 == 0)
        {
            if(token > arr[0])
                arr[0] = token;
        }
        else
        {
            if(token > arr[1])
                arr[1] = token;
        }
        //check if the number is greater than what this has and if its even or odd
        std::cout << "Process " << rank << " received token " << token << " to process " << (rank -1)%size << std::endl;
    }

    MPI_Send(&arr, 2, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        MPI_Recv(&arr, 2, MPI_INT, size -1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if(token % 2 == 0)
        {
            if(token > arr[0])
                arr[0] = token;
        }
        else
        {
            if(token > arr[1])
                arr[1] = token;
        }
        //check if the number is greater than what this has and if its even or odd
        printf("The President is %d and Vice President is %d\n", arr[1], arr[0]);
        std::cout << "Process " << rank << " received token " << token << " from process " << size -1 << std::endl;
    }

    MPI_Finalize();
    return 0;
}
