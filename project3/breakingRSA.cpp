// //----------------------------------------------------------------------
// // 
// // Authors: N8 Swalley & K8 Edwards
// // Date: 1/21/2022
// //
// // Breaking RSA - Parallel Trial Division: 
// //      This MPI program accepts an integer stored in a data fileas input, 
// //      and produces its prime factorization in an output file. This program
// //      parallelizes trial division. 
// //
// //----------------------------------------------------------------------

// #include <iostream>
// #include <cmath>
// #include <vector>
// #include <fstream>
// #include <sstream> 
// #include "mpi.h"
// #include "smallprimes.h"
// #include "bigint.h"

// using namespace std;

// vector<int> vect;

// long int countprimes(int id, int np, int * arr, int128 n)
// {
// 	long int sum=0;
//     int sqrtn=sqrt(n);
	
//     findsmallprimes(sqrtn);

//     for (long int start = sqrtn + 1 + id*sqrtn; start<n; start+=np*sqrtn)
// 	{
// 		long int stop = start+sqrtn-1;
//         if(stop>n) stop =n;
// 		//cout << "start="<< start <<" stop="<< stop << endl;
// 		//cout << "id=" << id << endl;

//         bool it[stop-start+1];
//         int itlen=stop-start+1;
// 	    // fill with all trues
// 	    for(int i=0; i<itlen; i++) it[i]=true;
	    
//         //cross off
// 	    for(int i=0; primes[i]<=sqrt(stop) && i<primeslen; i++)
//         {
//             int d = primes[i];
// 			//cout << "id="<<id<<"d="<<d<<" primes[i]="<<primes[i]<<endl;
//             long int q;
//             if (start % d == 0) q=start;
//             else q=start+(d-(start%d));

//             for ( ; q<= stop; q+=d ) it[q-start]=false;
//          }
// 	    for(int i=0; i<itlen; i++) if(it[i]) sum++;
// 	}
// 	return sum;
// }

// int main(int argc, char *argv[])
// {
// 	MPI_Init(&argc,&argv);
	
// 	int id;
// 	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	
// 	int np;
// 	MPI_Comm_size(MPI_COMM_WORLD,&np);
	
// 	long int limit;
// 	MPI_Status status;

//     int128 n = 0;
//     ifstream inFile;
// 	ofstream outFile;
// 	string str = " ";
// 	int * arr;

// 	if(id==0)  // gather the input
// 	{
//         inFile.open("testinput.txt");
//         while (inFile >> str) 
//         {
// 			stringstream ss(str);
//             cout << str<<endl;
// 			ss >> n;
				
//         }
// 		cout << "n is " << fixed << n <<endl;
// 		arr = new int[str.size()];
// 		int x = str.size();
// 		int y = 0;

// 		while (y != x)
// 		{

// 			arr[y] = str[y] - '0';

// 			y++;

// 		}
// 		// for(int i=0; i< str.size(); i++)
// 		// {
// 		// 	//cout << arr[i] <<endl;
// 		// }
//         //MPI_Bcast(&arr,1,MPI_INT,0,MPI_COMM_WORLD);
//         inFile.close();
// 	}

	
// 	//MPI_Bcast(&arr,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
	
// 	// cout << "Hello world!  I am "<<id<<" out of "<< np
// 	//   <<" limit="<<limit 
// 	//   << " sum=" << sum << endl;
	
// 	long int sum=0;

// 	//sum=countprimes(id,np,arr, n);

// 	// cout << "Hello world!  I am "<<id<<" out of "<< np << " limit="<<limit 
// 	//   << " sum=" << sum << endl;
	
// 	long int bigsum=0;
// 	//MPI_Allreduce(&sum,&bigsum,1,MPI_LONG,MPI_SUM,MPI_COMM_WORLD);
//     bigsum += primeslen;
	
// 	if(id==0)
// 	{
// 		outFile.open("testoutput.txt");
// 		outFile << n <<endl;
// 		for(int i=0; i < str.size(); i++)
// 		{
// 			outFile << arr[i];
// 			//cout << vect[i];
// 		}
// 		outFile.close();
// 	}
	
// 	MPI_Finalize(); // finish
// 	return 0;
// }