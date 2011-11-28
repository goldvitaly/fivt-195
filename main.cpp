#include "external_sort.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
template <typename Comp>
void test_integers(int n, int blocksize){
	std::fstream file("testint",std::ios_base::out|std::ios_base::trunc);
	std::vector<int> v(n);
	for(int i=0;i<n;++i){
		v[i]=rand();
		file<<v[i]<<' ';
	}
	std::sort(v.begin(),v.end());

	file.close();
	default_external_sort<int>(blocksize,"testint");
	
	file.open("testint", std::ios_base::in);
	for(int i=0;i<n;++i){
		int a;
		file>>a;
		if(a!=v[i]){
			std::cout<<"error index:"<<i<<" expected: "<<v[i]<<" found: "<<a<<std::endl;
			return;
		}		
	}
	std::cout<<"test int OK\n";
}


int main(int argc, char** argv) {
	test_integers(100000,1000);
	return 0;
}

