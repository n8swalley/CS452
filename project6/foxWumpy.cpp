#include <iostream>
#include <fstream>
#include "matrix.h"
#include "mpi.h"
using namespace std;
// to compile 
// mpicxx hello.cpp
// mpirun -np 8 ./a.out
// to run with time: date; mpirun -np 4 ./a.out; date

void mult(int id, int np) {
    int n; //size of the matrix

    MPI_Status status;

    double(*m1)[maxrowsize] = new double[maxrowsize][maxrowsize];
    double(*m2)[maxrowsize] = new double[maxrowsize][maxrowsize];
    double(*answer)[maxrowsize] = new double[maxrowsize][maxrowsize];

    //new declared for this project
    double sendRow[maxrowsize];
    double receiveRow[maxrowsize];

    if(id == 0) {
        ifstream fm1, fm2;
        fm1.open("matrix1.txt");
        fm2.open("matrix2.txt");
        readmatrix(m1, n, fm1);
        readmatrix(m2, n, fm2);
        fm1.close();
        fm2.close();
    }

    // each processor has row i of matrix a
    // then it multiplies with b

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    //MPI_Bcast(m1, maxrowsize*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    if(id == 0) {
        for(int i = 1; i < np; i++){
            for(int j = 0; j < n; j++){
                sendRow[j] = m1[i][j];
            }
            MPI_Send(&sendRow, n, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
    }
    else {
        MPI_Recv(&receiveRow, n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }


    //sending the vectors from 0
    if(id == 0){
        for(int i = 0; i < n; i++){
            receiveRow[i] = m1[0][i];
        }
                for(int i = 0; i<n; i++){
            cout << receiveRow[i] << " " ;
        }
    }

    MPI_Bcast(m2, maxrowsize*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            answer[i][j] = 0;
        }
    }

    // helps keep track of what row we're on
    int round = 1;

    // for(int i = 0; i < n; i++){
    //     answer[id][i] = receiveRow[i] * m2[id][i];
    // }

    for(int j = 0; j < n; j++) {
        for(int k = 0; k < n; k++) {
            answer[id][j] += receiveRow[k] * m2[k][j];
        }
    }

            //     for(int j = 0; j < n; j++) {
    //         answer[i][j] = 0;
    //         for(int k = 0; k < n; k++) {
    //             answer[i][j] += m1[i][k] * m2[k][j];
    //         }
    //     }


    
    double(*biganswer)[maxrowsize] = new double[n][maxrowsize];

    MPI_Reduce(answer, biganswer, n * maxrowsize, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(id == 0) {
        ofstream f("output.txt");
        writematrix(biganswer, n, f);
    }

    delete[]biganswer;
    delete[]m1;
    delete[]m2;
    delete[] answer;
}

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);
	
	// who are we?
	int id;
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	
	// how big is the group?
	int np;
	MPI_Comm_size(MPI_COMM_WORLD, &np);

    mult(id, np);
	
	MPI_Finalize(); // finish
	return 0;