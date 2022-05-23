//----------------------------------------------------------------------
// 
// Author: N8 Swalley & K8 Edwards
// Date: 1/21/2022
//
// Matrix Multiplixation using MPI
// 
//
//----------------------------------------------------------------------

// To compile and run:
// mpicxx -o run hello.cpp
// mpirun -q -np 8 run

// This program reads in matricies from matrixA.txt and matrixB.txt
// Make sure that the number of processors is equal to the dimension of the matrix (n)
// make sure that maxrowsize in matrix.h is equal to n

#include <iostream>
#include <fstream>
# include <stdlib.h>
#include "matrix.h"
#include "mpi.h"

using namespace std;

void matrixMultiplication( int n, vec aRows, matrix b, vec cRows, matrix c)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        for( int j = 0; j < n; j++)
        {
            c[i][j] = 0;
            sum += aRows[j] * b[j][i];
        }
        cRows[i] = sum;
        sum = 0;
    }
    
}



int main(int argc, char *argv[])
{
    MPI_Init(&argc,&argv);

    int id;
    MPI_Comm_rank(MPI_COMM_WORLD,&id);

    int np;
    MPI_Comm_size(MPI_COMM_WORLD,&np);

    int n;

    if (id == 0)
    {
        cout<< "Please enter N: ";
        cin >> n;
    }

    if (np != n) 
    {
        if (id == 0)
        {
            cout<< "Number of processors is not divisible by the matrix size!\n" << endl;
            MPI_Finalize();
            exit(-1);
        }
    }
    MPI_Bcast (&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // NOTE: make sure that maxrowsize is set to the value of n in matrix.h!

    // READ IN matrixA 
    double matrixA[n][maxrowsize];
    ifstream f("matrixA.txt");
    readmatrix(matrixA,n,f);
    f.close();

    // READ IN matrixB
    double matrixB[n][maxrowsize];
    ifstream f2("matrixB.txt");
    readmatrix(matrixB,n,f2);
    f2.close();

    double matrixC[n][maxrowsize];
    double aRows[n];
    double cRows[n];

    // scatter rows of first matrix to different processors
    MPI_Scatter(matrixA, n*n/np, MPI_DOUBLE, aRows, n*n/np, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // I don't think i need to broadcast B!
    // MPI_Bcast(matrixB, n*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // MPI_Barrier(MPI_COMM_WORLD);

    matrixMultiplication(n,aRows, matrixB, cRows, matrixC);
    
    MPI_Gather(cRows, n*n/np, MPI_DOUBLE, matrixC, n*n/np, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    //MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    

    // answer stored in output.txt
    if (id == 0)
    {
        cout<<"The product of matrixA and matrixB was stored in 'output.txt'\n";
        ofstream f3("output.txt");
        writematrix(matrixC,n,f3);
    }
  }



