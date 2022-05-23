//----------------------------------------------------------------------
// 
// Author: N8 Swalley & K8 Edwards
// Date: 4/2/2022
//
// PeeMerge -> Parallel mergesort!!!
// 
//
//----------------------------------------------------------------------

// To compile and run:
// mpicxx -o run peeMerge.cpp
// mpirun -q -np 8 run
// to run with time: date; mpirun -np 4 ./a.out; date

#include<iostream>
#include<fstream>
#include <cmath>
#include "mpi.h"
#include "quicksort.h"

using namespace std;

int main(int argc, char *argv[])
{
	MPI_Init(&argc,&argv);
	
	int id;
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	
	int np;
	MPI_Comm_size(MPI_COMM_WORLD,&np);
	
    MPI_Status status;

	long n;
	int * myArray;
	ifstream fileInput;
	
	if(id==0)
    { 
		fileInput.open("input.txt");
		fileInput >> n;
	}
    
	MPI_Bcast(&n,1,MPI_LONG,0,MPI_COMM_WORLD); 

	int myNum = n / (np/2+1); 
    int left = id * 2 + 1;
	int right = id * 2 + 2;
	int myParent = ceil((double) id / 2) - 1;
	
	if(id==0)
    {
		myArray = new int[n];

		for(int i=0; i<n; i++) 
        {
			fileInput >> myArray[i];
        }
		fileInput.close();
		
		int temp = np / 2;
		for(int i=0; i < n; i += myNum , temp++)
        { 
			MPI_Send(myArray+i, myNum, MPI_INT, temp, 0, MPI_COMM_WORLD);
        }
	}
	int * peeArray = new int[n];
	
	for(int i=0; i<n; i++)
    {
		peeArray[i]= -1; 
    }
	
	if(id >= np / 2)
    { 
		MPI_Recv(peeArray, myNum, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		quicksort(peeArray, myNum) ; 
 	}
	
	int leftData(0);
    int rightData(0);
    int index(0);
    int peeVar(0);

	int first = np / 2;
	int last = np - 1;

	int fVar = first / 2;
	int lVar = first - 1;
	
	for(int i=0; i < log2(np+1)-1; i++)
    {
		index = 0;
		if(first <= id && id <= last)
        { 
			MPI_Send(&peeArray[index++], 1, MPI_INT, myParent, 0, MPI_COMM_WORLD);
        }
		else if(fVar <= id && id <= lVar)
        { 
            MPI_Recv(&leftData, 1, MPI_INT, left, 0, MPI_COMM_WORLD, &status);

			MPI_Recv(&rightData, 1, MPI_INT, right, 0, MPI_COMM_WORLD, &status);
		}
		
		for(int i=0; i < myNum * 2 ; i++)
        {
			if(fVar <= id && id <= lVar)
            { 
				if(leftData==-1)
                { 
					peeArray[index++]=rightData;
                    peeVar=1;
				}
				else if(rightData== -1)
                { 
                    peeArray[index++] = leftData;
				    peeVar=0;
				}
				else if(leftData<rightData)
                {
                    peeArray[index++] = leftData;
				    peeVar=0;
				}
				else
                { 
					peeArray[index++] = rightData;
                    peeVar=1;
				}
				MPI_Send(&peeVar, 1, MPI_INT, left, 0, MPI_COMM_WORLD); 
				MPI_Send(&peeVar, 1, MPI_INT, right, 0, MPI_COMM_WORLD);
			}
			
			if(first <= id && id <= last)
            { 	
				MPI_Recv(&peeVar, 1, MPI_INT, myParent, 0, MPI_COMM_WORLD, &status);

				if(peeVar == 0 && id % 2 == 1) 
					MPI_Send(&peeArray[index++], 1, MPI_INT, myParent, 0, MPI_COMM_WORLD);
				else if(peeVar == 1 && id % 2 == 0)
					MPI_Send(&peeArray[index++], 1, MPI_INT, myParent, 0, MPI_COMM_WORLD);
			}
			else if(fVar <= id && id <= lVar)
            {
				if(peeVar==0)
					MPI_Recv(&leftData, 1, MPI_INT, left, 0, MPI_COMM_WORLD, &status);
				else
					MPI_Recv(&rightData, 1, MPI_INT, right, 0, MPI_COMM_WORLD, &status);
			}
		} 
        fVar = fVar / 2;
		lVar = lVar / 2 - 1;
		myNum = myNum * 2 ; 
		first = first / 2;
		last = last / 2 - 1;
	}
	if(id==0)
    {
		ofstream fileOutput;
		fileOutput.open("sortedlist.txt");
		for(int i=0; i<n; i++)
			fileOutput << peeArray[i] << " ";
	}
	
	MPI_Finalize();
	return 0;
}