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
	
	long n;
	int * bigarray;
	ifstream fin;
	
	if(id==0){ 
		fin.open("input.txt");
		fin >> n;
	}
    
	MPI_Bcast(&n,1,MPI_LONG,0,MPI_COMM_WORLD); 

	//data in leaf siz
	int botNum=n/(np/2+1); 
	//find parent and child
	int parent = ceil((double) id/2)-1;


	int left = id*2+1;
	int right = id*2+2;
	
	if(id==0){
		bigarray=new int[n];
		for(int i=0; i<n; i++) 
			fin >> bigarray[i];
		fin.close();
		
		int goTo = np/2;
		for(int i=0; i<n; i+=botNum, goTo++) 
			MPI_Send(bigarray+i, botNum, MPI_INT, goTo, 0, MPI_COMM_WORLD);
	}
	
	//make a list for each proccessor
	int * partialArray = new int[n];
	for(int i=0; i<n; i++)
		partialArray[i]=-1; 
	
	MPI_Status status;
	if(id>=np/2){ 
		MPI_Recv(partialArray, botNum, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	
		quicksort(partialArray, botNum);
	}
	
	int ldata, rdata, index, pleaseMayIHave;


	//find important nodes
	int first=np/2;
	int last=np-1;

	int firstget=first/2;
	int lastget=first-1;
	
	//hight of tree 
	for(int i=0; i<log2(np+1)-1; i++){
		index=0;
		if(first <= id && id <= last) 
			MPI_Send(&partialArray[index++], 1, MPI_INT, parent, 0, MPI_COMM_WORLD);
		else if(firstget <= id && id <= lastget){ 

			MPI_Recv(&ldata, 1, MPI_INT, left, 0, MPI_COMM_WORLD, &status);

			MPI_Recv(&rdata, 1, MPI_INT, right, 0, MPI_COMM_WORLD, &status);
		}
		
		for(int i=0; i<botNum*2; i++){
            //pleaseMayIHave decided who sends the next number 
			if(firstget <= id && id <= lastget){ 
				if(ldata==-1){ 
					partialArray[index++]=rdata; 
					pleaseMayIHave=1; 
				}
                //if left is empty, go right
				else if(rdata==-1){ 
					partialArray[index++]=ldata;
					pleaseMayIHave=0;
				}
                //take smaller and repopulate
				else if(ldata<rdata){ 
	
					partialArray[index++]=ldata;
					pleaseMayIHave=0;
				}
                //if the right one is bigger, do that  again
				else{ 
					partialArray[index++]=rdata;
					pleaseMayIHave=1;
				}
				MPI_Send(&pleaseMayIHave, 1, MPI_INT, left, 0, MPI_COMM_WORLD); 
				MPI_Send(&pleaseMayIHave, 1, MPI_INT, right, 0, MPI_COMM_WORLD);
			}
			
			if(first <= id && id <= last){ 	
                
				MPI_Recv(&pleaseMayIHave, 1, MPI_INT, parent, 0, MPI_COMM_WORLD, &status);

				if(pleaseMayIHave==0 && id%2==1) 
					MPI_Send(&partialArray[index++], 1, MPI_INT, parent, 0, MPI_COMM_WORLD);
				else if(pleaseMayIHave==1 && id%2==0)
					MPI_Send(&partialArray[index++], 1, MPI_INT, parent, 0, MPI_COMM_WORLD);
			}
			else if(firstget <= id && id <= lastget){ 
        
				if(pleaseMayIHave==0)
					MPI_Recv(&ldata, 1, MPI_INT, left, 0, MPI_COMM_WORLD, &status);

				else
					MPI_Recv(&rdata, 1, MPI_INT, right, 0, MPI_COMM_WORLD, &status);
			}
		}
		//update who is sending 

		first=first/2;
		last=last/2-1;
		firstget=firstget/2;
		lastget=lastget/2-1;
		botNum=botNum*2;
	}
	if(id==0){ //id 0 prints sorted list to file
		ofstream fout;
		fout.open("sortedlist.txt");
		for(int i=0; i<n; i++)
			fout << partialArray[i] << " ";
	}
	
	MPI_Finalize();
	return 0;
}