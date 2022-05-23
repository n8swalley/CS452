#ifndef _MATRIX
#define _MATRIX

#include<fstream>
#include<cmath>
#include<cstdlib>
using namespace std;

// MAXROWSIZE MUST BE EQUAL TO N!
const int maxrowsize=2;

typedef double matrix[][maxrowsize];
typedef double vec[];

// multiply matrix/vector

void mvmultiply( int n, matrix m, vec v, vec answer )
{
  for(int i=0; i<n; i++)
  {
    answer[i]=0;
    for(int j=0; j<n; j++)
      answer[i]+= m[i][j]*v[j];
  }
}

// random fill functions

void makematrix( int n, matrix m)
{
  for(int i=0; i<n; i++)
    for(int j=0; j<n; j++)
      m[i][j]= -5+(rand()%11) ;
}

void makevector( int n, vec v )
{
  for(int i=0; i<n; i++)
    v[i]= -5+(rand()%11);
}

// read and write functions


void readmatrix( matrix m, int & n, ifstream & f)
{
  f >> n;
  for(int i=0; i<n; i++)
    for(int j=0; j<n; j++)
      f >> m[i][j];
}

void writematrix( matrix m, int n, ofstream & f)
{
  f << n << endl;
  for(int i=0; i<n; i++)
  {
    for(int j=0; j<n; j++)
      f << m[i][j] <<" ";
    f << endl;
  }
}

void readvector( vec v, int & n, ifstream & f)
{
  f >> n;
  for(int i=0; i<n; i++)
      f >> v[i];
}

void writevector( vec v, int n, ofstream & f)
{
  f << n << endl;
  for(int i=0; i<n; i++) f << v[i] <<" ";
  f << endl;
}

#endif