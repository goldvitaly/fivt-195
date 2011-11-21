//Hello from parent branch!

#define MEMCHK

#include "heap/binheap.h"
#include <cstdlib>
#include <iostream>

#ifdef MEMCHK
	#include <mcheck.h>
#endif

using namespace std;

int main(){
	#ifdef MEMCHK
		mtrace();
	#endif
	srand(35);
	binheap h;
	int sz = 50;
	for(int i = 0; i < sz; i++){
		int n=rand()%100;
		cout << n << ' ';
		h.push(n);
		//h.print();
	}
	cout<<endl;
	for(int i = 0; i < sz; i++){
		cout << h.top() << ' ';
		h.pop();
		//h.print();
	}
	cout<<endl;
	#ifdef MEMCHK
		muntrace();
	#endif
	return 0;
}
