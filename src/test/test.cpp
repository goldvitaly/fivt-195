<<<<<<< HEAD
//Hello from merged branch!
=======
//Hello from merged branch!
>>>>>>> Zakhar.TemplateHeap

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
	binheap<double> h;
	int sz = 50;
	for(int i = 0; i < sz; i++){
		double n=rand()%100/50.0;
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
