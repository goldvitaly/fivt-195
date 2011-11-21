//Hello from child branch!

#define MEMCHK

#include "heap/binheap.h"
#include <cstdlib>
#include <iostream>
#include <mcheck.h>

using namespace std;

int main(){
	mtrace();
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
	muntrace();
	return 0;
}
