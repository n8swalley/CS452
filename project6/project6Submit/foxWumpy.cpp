//----------------------------------------------------------------------
// 
// Author: N8 Swalley & K8 Edwards
// Date: 4/2/2022
//
// Fox and Wumpus
// 
//
//----------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include "matrix.h"
#include "mpi.h"

using namespace std;

// To compile and run:
// mpicxx -o run hello.cpp
// mpirun -q -np 8 run
// to run with time: date; mpirun -np 4 ./a.out; date

void matrixMultiplication(int id, int np);


int main(int argc, char *argv[]) 
{
	MPI_Init(&argc, &argv);
	
	int id;
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	
	int np;
	MPI_Comm_size(MPI_COMM_WORLD, &np);

    matrixMultiplication(id, np);
	
	MPI_Finalize(); // finish
	return 0;
}

void matrixMultiplication(int id, int np) 
{
    int n;

    MPI_Status status;

    double rRow[maxrowsize];
    double sRow[maxrowsize];

    double(*m1)[maxrowsize] = new double[maxrowsize][maxrowsize];
    double(*m2)[maxrowsize] = new double[maxrowsize][maxrowsize];
    double(*answer)[maxrowsize] = new double[maxrowsize][maxrowsize];


    if(id == 0) 
    {
        ifstream fm1, fm2;

        fm1.open("matrixA.txt");
        readmatrix(m1, n, fm1);
        fm1.close();

        fm2.open("matrixB.txt");
        readmatrix(m2, n, fm2);
        fm2.close();
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(id == 0) 
    {
        for(int i = 1; i < np; i++)
        {
            for(int j = 0; j < n; j++)
            {
                sRow[j] = m1[i][j];
            }
            MPI_Send(&sRow, n, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
    }
    else 
    {
        MPI_Recv(&rRow, n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }

    if(id == 0)
    {
        for(int i = 0; i < n; i++)
        {
            rRow[i] = m1[0][i];
        }

        for(int i = 0; i<n; i++)
        {
            //cout << rRow[i] << " " ;
        }
        cout << "Check output.txt for answer!! ;) \n";
    }

    MPI_Bcast(m2, maxrowsize*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for(int i = 0; i < n; i++) 
    {
        for(int j = 0; j < n; j++) 
        {
            answer[i][j] = 0;
        }
    }

    for(int j = 0; j < n; j++) 
    {
        for(int k = 0; k < n; k++) 
        {
            answer[id][j] += rRow[k] * m2[k][j];
        }
    }

    double(*realAnswer)[maxrowsize] = new double[n][maxrowsize];

    MPI_Reduce(answer, realAnswer, n * maxrowsize, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(id == 0) 
    {
        ofstream f("output.txt");
        writematrix(realAnswer, n, f);
    }

    delete[] answer;
    delete[]realAnswer;
    delete[]m1;
    delete[]m2;
}
