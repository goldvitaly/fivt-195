#include "binheap.h"
#include <cstdlib>
#include <iostream>

using namespace std;

int main(){
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
	return 0;
}
