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
	muntrace();
	return 0;
}
