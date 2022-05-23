
  
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

ifstream inFile;
ofstream outFile;


int S =0;
int R=0;
int L=0;
int H=0;
int r=0;
int p=0;
int c=2;

// void showq(queue<int> gq)
// {
//     queue<int> g = gq;
//     while (!g.empty()) {
//         cout << '\t' << g.front();
//         g.pop();
//     }
//     //cout << '\n';
// }

void countprimes(int id, int np, int128 n)
{
    //cout << "N IS : " << n << endl;
    long int sqrtn=sqrt(n);
    //cout << "sqrtN IS : " <<sqrtn << endl;
    findsmallprimes(sqrtn);

    long int bStart = sqrtn + 1 + id * sqrtn;
    long int bStop = bStart + sqrtn - 1;
    long int itlen = bStop - bStart + 1;
    bool it[itlen];
    // S = (n-(sqrtn+1)) / np;
    // R = (n-(sqrtn+1)) % np;
    // L = sqrtn + id*S +1;
    // H = L+S-1; 

    // if (r == np-1)
    // {
    //     H += R;
    // }




    // for (c = 2; c <= sqrtn; c++)
    //     {
    //         list1[c] = 0;
    //     }

    //     for (c = L; c <= H; c++)
    //     {
    //         list2[c-L] = 0;
    //     }

    //     for (c = 2; c <= sqrtn; c++)
    //     {
    //         if (list1[c] == 0)
    //         {
    //             for (int i = c+1; i <= sqrtn; i++)
    //             {
    //                 if (i%c == 0)
    //                 {
    //                     list1[i] = 1;
    //                 }
    //             }

    //             for (int i = L; i<= H; i++)
    //             {
    //                 if(i%c == 0)
    //                 {
    //                     list2[i-L] = 1;
    //                 }
    //             }
    //         }
    //     }

    //     if (id == 0)
    //     {
    //         for(int i = 2; i <= sqrtn; i++)
    //         {
    //             if(list1[i] == 0)
    //             {
    //                 cout<<"PRIME1: "<<list1[i]<<endl;
    //             }
    //         }
    //         for (int i = L; i <= H; i++)
    //         {
    //             if(list2[i-L] == 0)
    //             {
    //                 cout<<"PRIME2: "<< list2[i]<<endl;
    //             }
    //         }

    //         for (id = 1; id <= np-1; id++)
    //         {
    //         L = sqrtn + id * S + 1;
    //         H = L + S-1;
    //         if (id == np-1)
    //         {
    //             H += R;
    //         }
    //         MPI_Recv(list2, H-L+1, MPI_INT, id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //         }
    //     }
    //     else
    //     {
    //         MPI_Send(list2, H-L+1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    //     }
    for(long int start = bStart; start < n; start += np * sqrtn)
    {
        //cout<< "HERE1"<< endl;
        long int stop  = start + sqrtn - 1;

        // find primes between start and stop
        if(stop>n) 
            stop=n;
        


        // fill with all trues
        for(long int i=0; i<itlen; i++)
            it[i]=true;

        
        //cross off
        for(long int i=0; primes[i] <= sqrt(stop) && i < primeslen; i++)
        {
            long int d = primes[i];
            long int q;
    
            if(start % d == 0)
                q = start;
            else 
                q= start + (d - (start % d));
            
            for( ; q <= stop; q += d) 
                it[q - start] = false;
        }
        
    //     for(int i=0; i < itlen; i++)  
    //     {
            
    //         if(it[i])
    //         {
                
                
    //             //cout<<"PRIMES"<< start+i<<endl;
    //             //q.push(start+i);
    //             //sum++; 

    //         }
    //     }
    //   }
    }


    for(long int i=0; i < primeslen; i++)
    {
        primeVector.push_back(primes[i]);
    }

     if (id == 0)
     {
        for (long int i = 0; i < primeVector.size(); i++)
        {
            //cout<<"here222" << endl;
            if ((n % primeVector[i] == 0))
            {
                primeVector2.push_back(primeVector[i]);
                cout << primeVector[i] << " is part of the factorization"<<endl;
            }
        }
    }

    long int newN = n;
    bool go = true;
    for (long int i = 0; i < primeVector2.size(); i++)
    {
        go = true;
        while (go == true)
        {
            if (newN % primeVector2[i] == 0 && newN != 1)
            {
                //cout<<"HERE2"<<endl;
                newN = newN / primeVector2[i];
                cout << primeVector2[i] << " * ";// << newN;
                if (id == 0)
                {
                    outFile.open("testoutput.txt");
                    outFile << primeVector2[i] << " * ";
                }
            }
            else
            {
                go = false;
                if (id == 0)
                {
                    outFile << newN;
                }
                cout << newN <<endl;
            }
        }
    }
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
    
    int128 n = 0;

    MPI_Status status;
    if(id==0)  // gather the input
    {
        inFile.open("testinput.txt");
        inFile >> n;
		cout << "n is " << fixed << n <<endl;
        
    }
    
    //MPI_Bcast(&n,1,MPI_LONG,0,MPI_COMM_WORLD);
    countprimes(id,np,n);
    

    //MPI_Allreduce(&sum,&bigsum,1,MPI_LONG,MPI_SUM,MPI_COMM_WORLD);
    //bigsum += primeslen;

    //int divisors=limit;
    
     //showq(q);

        
        //int numberofprimes = q.size();

            // if (divisors==1)
            // {
            //     break;
            // }
            // int temp = q.front();
            
            // while(divisors % temp == 0)
            // {
            //     outFile << n <<endl;
            //     divisors = divisors / temp;
            //     outFile << temp;
            //     factors.push(temp);
            //     //cout<<temp<<endl;
            // }
           // q.pop();
        //}
		

    //showq(factors);
   // }
    
    
    MPI_Finalize(); // finish
    return 0;
}
