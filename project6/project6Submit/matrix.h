#ifndef _MATRIX
#define _MATRIX

#include<fstream>
#include<cmath>
#include<cstdlib>
using namespace std;

const int maxrowsize=1000;

typedef double matrix[][maxrowsize];

// multiply matrix & matrix

void matrixmultiply(int n, matrix m1, matrix m2, matrix answer) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            answer[i][j]=0;
            for(int k=0; k<n; k++)
                answer[i][j] += m1[i][j] * m2[i][j];
        }
    }
}

// random fill functions
void makematrix(int n, matrix m) {
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            m[i][j]= (rand()%2) ;
}


// read and write functions

void readmatrix(matrix m, int & n, ifstream & f) {
    f >> n;
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            f >> m[i][j];
}

void writematrix(matrix m, int n, ofstream & f) {
    f << n << endl;
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++)
            f << m[i][j] << " ";
        f << endl;
    }
}


#endif