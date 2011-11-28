

#define MEMCHK

#include "heap/Binheap.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>

#ifdef MEMCHK
	#include <mcheck.h>
#endif

using namespace std;

int main(){
	#ifdef MEMCHK
		mtrace();
	#endif
	
	srand(35);
	
	Binheap<double> h1;
	Binheap<double> h2;
	int sz = 100000;
	int maxval = 1000000;
	
	double* testarray = new double[sz];
	
	for(int i = 0; i < sz; i++){
		double n=(double)rand()/RAND_MAX;
		if(i % 2)
			h1.push(n);
		else
			h2.push(n);
		testarray[i] = n;
	}
	
	sort(testarray,testarray + sz);
	reverse(testarray,testarray + sz);
	
	h1.merge(&h2);
	
	for(int i = 0; !h1.empty(); i++){
		double heaptop = h1.top();
		if(i >= sz){
			cerr << "Wrong size manipulation" << endl;
			return -1;
		}
		if(heaptop != testarray[i]){
			cerr << "Wrong value in heap (index " << i << "): " << heaptop << "(need " << testarray[i] << ")" << endl;
			return -1;
		}
		h1.extractMax();
	}
	
	cerr << "Test successfull" << endl;
	
	#ifdef MEMCHK
		muntrace();
	#endif
	return 0;
}
