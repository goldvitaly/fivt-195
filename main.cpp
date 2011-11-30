#include "external_sort.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <string>
#include "ByteFileSource.h"

template <typename T, typename F>
void test(int n, int blocksize, F generator, const std::string& testName){
	std::vector<T> v(n);
	std::generate(v.begin(),v.end(),generator);
	std::fstream file("test",std::ios_base::out|std::ios_base::trunc);
	
	for(auto i=v.begin();i!=v.end();++i){
		file<<(*i)<<' ';
	}
	std::sort(v.begin(),v.end());
	
	file.close();
	default_external_sort<T>(blocksize,"test");

	file.open("test", std::ios_base::in);
	for(auto i=v.begin();i!=v.end();++i){
		T a;
		file>>a;
		if(a!=(*i)){
			std::cout<<"error index:"<<(i-v.begin())<<" expected: "<<(*i)<<" found: "<<a<<std::endl;\
			file.close();
			return;
		}
	}
	file.close();
	std::cout<<"test \""<<testName<<"\" OK\n";
}

std::string generate_string(){
	std::string s;
	for(int i=0,e=rand()%5+1;i<e;++i){
		s+=rand()%26+'a';
	}
	return s;
}


void test_binary_ints(int n,int blocksize){
	std::vector<unsigned> v(n);
	std::generate(v.begin(),v.end(),rand);
	ByteFileSource file("testinput");

	for(auto i=v.begin();i!=v.end();++i){
		file.read(*i);
	}
	std::sort(v.begin(),v.end());

	file.toStart();
	external_sort<unsigned,ByteFileSource>(
		blocksize,
		file,
		FileStorageManager<unsigned, ByteFileSource>(),
		StdSort<unsigned>()
	);

	file.toStart();
	for(auto i=v.begin();i!=v.end();++i){
		unsigned a;
		file.read(a);
		if(a!=(*i)){
			std::cout<<"error index:"<<(i-v.begin())<<" expected: "<<(*i)<<" found: "<<a<<std::endl;\
			return;
		}
	}
	std::cout<<"test binary input/output works OK\n";
	
}

int main() {
	test<int>(100000,1000,rand,"Integers");
	test<std::string>(10000,913,generate_string,"Strings and not full groups");
	test_binary_ints(100000,1230);



	return 0;
}

