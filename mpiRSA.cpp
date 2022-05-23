//----------------------------------------------------------------------
// 
// Authors: N8 Swalley & K8 Edwards
// Date: 1/21/2022
//
// Breaking RSA - Parallel Trial Division: 
//      This MPI program accepts an integer stored in a data fileas input, 
//      and produces its prime factorization in an output file. This program
//      parallelizes trial division. 
//
//----------------------------------------------------------------------
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream> 
#include "mpi.h"
#include "smallprimes.h"
#include "bigint.h"
#include <queue>

using namespace std;

queue <int> q; //holds prime numbers
queue<int> factors; 
vector<int> primeVector;
vector<int> primeVector2;

// void showq(queue<int> gq)
// {
//     queue<int> g = gq;
//     while (!g.empty()) {
//         cout << '\t' << g.front();
//         g.pop();
//     }
//     //cout << '\n';
// }

long int countprimes(int id, int np, long int n)
{
    long int sum=0;
    int sqrtn=sqrt(n);
    findsmallprimes(sqrtn);

    long int bStart = sqrtn + 1 + id * sqrtn;
    long int bStop = bStart + sqrtn - 1;



    int itlen = bStop - bStart + 1;
    bool it[itlen];

    // if(id==0){
    //     for(int l=0; l<truecount; l++){
    //         //cout<<"k is "<<l<< "number is "<<primes[l]<< endl;
    //         q.push(primes[l]);
    //     }
    // }
    //cout << (primes[1])<< endl;

    for(long int start = bStart; start < n; start += np * sqrtn)
    {
        long int stop  =start + sqrtn - 1;

        // find primes betweeb start and stop
        if(stop>n) 
            stop=n;
        


        // fill with all trues
        for(int i=0; i<itlen; i++)
            it[i]=true;
        
        //cross off
        for(int i=0; primes[i] <= sqrt(stop) && i < primeslen; i++)
        {
            int d = primes[i];
            long int q;
    
            if(start % d == 0)
                q = start;
            else 
                q= start + (d - (start % d));
            
            for( ; q <= stop; q += d) 
                it[q - start] = false;
        }
        
        for(int i=0; i < itlen; i++)  
        {
            if(it[i])
            {
                //cout<<"PRIMES"<< start+i<<endl;
                //q.push(start+i);
                sum++;  
            }
        }
      }


    for(int i=0; i < primeslen; i++)
    {
        primeVector.push_back(primes[i]);
           //cout<<"here3333" << endl;
       // q.push(primes[i]);
    }

     if (id == 0)
     {
        for (int i = 0; i < primeVector.size(); i++)
        {
            //cout<<"here222" << endl;
            if ((n % primeVector[i] == 0))
            {
                primeVector2.push_back(primeVector[i]);
                cout << primeVector[i] << " is part of the factorization"<<endl;
            }
        }
    }

    //cout<<"here7777 and primeVector2 size is " << primeVector2.size() << endl;
    long int newN = n;
    bool go = true;
    for (int i = 0; i < primeVector2.size(); i++)
    {
        //cout<<"HERE1"<<endl;
        go = true;
        while (go == true)
        {
            if (newN % primeVector2[i] == 0)
            {
                //cout<<"HERE2"<<endl;
                newN = newN / primeVector2[i];
                cout << primeVector2[i] << " * ";// << newN;
            }
            else{
                cout<< newN;
                go = false;
            }
        }
    }

    return sum;
}

int main(int argc, char *argv[])
{

    MPI_Init(&argc,&argv);
    
    // who are we?
    int id;
    MPI_Comm_rank(MPI_COMM_WORLD,&id);
    
    // how big is the group?
    int np;
    MPI_Comm_size(MPI_COMM_WORLD,&np);
    
    long int limit;
    int128 n = 0;
    ifstream inFile;
	ofstream outFile;
	string str = " ";
	int * arr;

    MPI_Status status;
    if(id==0)  // gather the input
    {
        inFile.open("testinput.txt");
        while (inFile >> str) 
        {
			stringstream ss(str);
            cout << str<<endl;
			ss >> n;
				
        }
		cout << "n is " << fixed << n <<endl;
		arr = new int[str.size()];
		int x = str.size();
		int y = 0;

		while (y != x)
		{
			arr[y] = str[y] - '0';
			y++;
		}
		// for(int i=0; i< str.size(); i++)
		// {
		// 	//cout << arr[i] <<endl;
		// }
        //MPI_Bcast(&arr,1,MPI_INT,0,MPI_COMM_WORLD);
        inFile.close();
        limit=n;
    }
    
    MPI_Bcast(&limit,1,MPI_LONG,0,MPI_COMM_WORLD);
    
    long int sum=0;
    
    sum=countprimes(id,np,limit);

    

    
    long int bigsum=0;
    MPI_Allreduce(&sum,&bigsum,1,MPI_LONG,MPI_SUM,MPI_COMM_WORLD);
    bigsum += primeslen;

    int divisors=limit;
    
     //showq(q);

    if(id==0)
    {
        int newN = n;
        //cout << "newN = " << newN;
        outFile.open("testoutput.txt");
        //int numberofprimes = q.size();
        for(int i=0; i < primeVector2.size(); i++)
        {
            while (newN % primeVector2[i] == 0)
            {
                //if (newN % primeVector2[i] == 0)
                //{
                    //cout<<"HERE2"<<endl;
                    newN = newN / primeVector2[i];
                    //cout << primeVector2[i] << " * ";
                    outFile << primeVector2[i] << " * ";
                    if (newN % primeVector[i] != 0)
                        outFile << newN;
                //}
                //else
                  //  go = false;
            }
        }
        outFile.close();
		

    //showq(factors);
    }
    
    
    MPI_Finalize(); // finish
    return 0;
}

