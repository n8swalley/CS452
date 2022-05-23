//----------------------------------------------------------------------------------------------------
// 
// Author: N8 Swalley & K8 Edwards
// Date: 1/14/2022
//
// HELLO MPI:
// This program uses MPI to sends an integer, initialized to 0, from process 0 to process 1. 
// Then each process i receives the message, adds i to the message, and passes it on to processs i+1. 
// The last one passes the message back to process 0, which then prints it, and of course "Hello".
//
//----------------------------------------------------------------------------------------------------
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

using namespace std;

// To compile and run:
// mpicxx -o run hello.cpp
// mpirun -q -np 8 run

int main (int argc, char * argv[]) 
{
    int myRank;         // My CPU number for this program
    int numCPUs;        // Number of CPUs that we have
    int num = 0;        // integer passed to next processor
    int numMessage = 0; // integer passed to next processor after being incremented

    // Start MPI
    MPI_Init(&argc, &argv);

    // Find out my rank!
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    // Find out the number of processes!
    MPI_Comm_size(MPI_COMM_WORLD, &numCPUs);

    if (myRank == 0) 
    {
        // Processor 0 sends message to processor 1
        MPI_Send(&num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        // Processor 0 receives message from last processor
        MPI_Recv(&numMessage, 1, MPI_INT, numCPUs-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // print final message
        cout << "Processor "<< myRank << " received "<< numMessage << " from processor " << numCPUs-1 << endl;
        cout << "=" << numMessage << " Hello!" << endl;
    }

    for(int i=1; i<numCPUs-1; i++) 
    {
        if(myRank == i)
        {
            // receive message from previous processor, add index i, send new message to next
            MPI_Recv(&num, 1, MPI_INT, i-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            cout << "Processor "<< i << " received " << num << " from processor " << i-1 << endl;
            numMessage = num + myRank;
            cout << num << "+" << myRank <<endl;

            MPI_Send(&numMessage, 1, MPI_INT, i+1, 0, MPI_COMM_WORLD);
        }
    }

    if (myRank == numCPUs-1)
    {
        // last processor sends message back to processor 0
        MPI_Recv(&num, 1, MPI_INT, numCPUs-2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout << "Processor "<< numCPUs-1 << " received " << num << " from processor " << numCPUs-2 << endl;
        numMessage = num+myRank;
        cout << num << "+" << myRank << endl;

        MPI_Send(&numMessage, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // Shut down MPI
    MPI_Finalize();

    return 0;
}