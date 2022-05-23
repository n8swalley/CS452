#ifndef _QUICK
#define _QUICK

using namespace std;

inline void swap(int &x, int &y)
{ int temp=x; x=y; y=temp; }

int partition(int x[], int start, int stop)
{
  int pivot=x[stop];
  int right=start;
  for(int i=start; i<stop; i++)
  {
    if(x[i]<pivot) { swap(x[i],x[right++]); }
  }
  swap(x[stop],x[right]);
  return right;
}

void quicksort(int x[], int start, int stop)
// recursive
{
  if(start>=stop) return; // size 0 or 1
  if(start+1==stop) // size 2
  {
    if(x[start]>x[stop]) swap(x[start],x[stop]);
  }
  // recursive case
  int randpivot=start+rand()%(stop-start+1);
  swap(x[randpivot],x[stop]);
  int pivotpos=partition(x,start,stop);
  quicksort(x,start,pivotpos-1);
  quicksort(x,pivotpos+1,stop);
}

void quicksort(int x[], int len)
{ quicksort(x,0,len-1); }

#endif
