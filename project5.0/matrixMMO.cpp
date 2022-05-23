//----------------------------------------------------------------------
// 
// Author: N8 Swalley & K8 Edwards
// Date: 3/15/2022
//
// Wumpus MMO Caves
// 
//
//----------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include "mpi.h"
#include "matrix.h"
using namespace std;

// To compile and run:
// mpicxx -o run hello.cpp
// mpirun -q -np 8 run

int main(int argc, char *argv[])
{
    MPI_Init(&argc,&argv);

    int id;
    MPI_Comm_rank(MPI_COMM_WORLD,&id);

    int np;
    MPI_Comm_size(MPI_COMM_WORLD,&np);


    int n;
    int p;

    if (id == 0)
    {
        cout<< "Please enter N (dimension of matrix): ";
        cin >> n;

        cout << "Please enter the power of the matrix: ";
        cin >> p;
    }


    if (id == 0)
    {
        if (np % n != 0) 
        {
            cout<< "N should be divisible by the number of processors!\n" << endl;
            MPI_Finalize();
            exit(-1);
        }
    }

    MPI_Bcast (&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // NOTE: make sure that maxrowsize is set to the value of n in matrix.h!

    // READ IN matrixA 
    double m1[n][maxrowsize];
    ifstream f("adjMatrix.txt");
    readmatrix(m1,n,f);
    f.close();

    // READ IN matrixB
    double m2[n][maxrowsize];
    ifstream f2("adjMatrix.txt");
    readmatrix(m2,n,f2);
    f2.close();

    //double m1[2][2] = { {0,1}, {2,3}};
    //double m2[2][2] = {{3,2}, {1,0}};

    double m3 [2][2]; //our final solution
    double temp [2][2];

    for(int x=0; x<n; x++)  
    {
            for(int y=0;y<n;y++)  
            {
                m3[x][y]=m1[x][y];
                cout<< m3[x][y]<<"   ";  
            }
            cout<<endl; 
        }

    // int exponent; 
    // cout<<"enter number of exponentions"<<endl;
    // cin>>exponent; // Exponentiation


    for(int q=1; q<p; q++)
    { 
        for(int x=0;x<n;x++)  
        {
            for(int y=0;y<n;y++)  
            {
                temp[x][y]=m3[x][y]; // we need to update temp otherwise we lose answer and cant contine
            }
        }

        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                m3 [i][j]=0; 
                for(int k = 0; k < n; k++)
                {
                    m3[i][j] += temp[i][k] * m1[k][j]; 
                }
            }
        }

    if (id == 0)
    {
        cout<<"The product of matrixA and matrixB was stored in 'output.txt'\n";
        ofstream f3("output.txt");
        writematrix(m3,n,f3);
        for(int x=0;x<n;x++)  //just printing
        {
            for(int y=0;y<n;y++)  
            {
                cout<<m3[x][y]<<"   ";  
            }
        cout<<endl;  
        }
    }


    }



return 0;
}
