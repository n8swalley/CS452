#include <iostream>
#include <fstream>

#include "matrix.h"

int main(int argc, char *argv[]) {
  int n;
  if(argc > 0) 
    n = atoi(argv[1]);
  else 
    n = 25;
  
  double m1[n][maxrowsize];
  makematrix(n, m1);
  ofstream f("matrix1.txt");
  writematrix(m1, n, f);

  double m2[n][maxrowsize];
  makematrix(n, m2);
  f.close(); 
  f.open("matrix2.txt");
  writematrix(m2, n, f);
  f.close();
  
  return 0;
}