#ifndef _PRIMES
#define _PRIMES

// function to find small primes
using namespace std;

long int *primes; // the array holding the small primes
long int primeslen; // length of that array

void findsmallprimes(long int limit) // use limit = sqrt(n)
{
	bool it[limit+1];
	// fill with all trues
	for(long int i=0; i<=limit; i++) it[i]=true;
	// 0 and 1 are not primes
	it[0]=it[1]=false;
	
	//cross off
	for(long int d=2; d*d<=limit; d++)
		if(it[d]) // then cross off with it
	      for(long int q=2*d; q<=limit; q=q+d) it[q]=false;
	
	long int truecount=0;
	for(long int i=0; i<=limit; i++) if(it[i]) truecount++;
	
	primes=new long int[truecount];
	primeslen=0;
	// fill the primes array
	for(long int i=0; i<=limit; i++) 
		if(it[i]) primes[primeslen++]=i;
}
#endif