#include <algorithm>
#include <queue>
#include <iostream>
#include <string>
#include <map>
#include <cassert>
#include "heap.h"

std::string genstr(){
	std::string a;
	for(int i=0;i<7;++i){
		a+=char(rand()%26+'a');
	}
	return a;
}
void genStressTest(){

	std::cout<< "void test(){"<<std::endl<<"LeftHeap<int,std::string,std::greater<int> > lh;"<<std::endl;
	int expectedSize=0;
	std::priority_queue<int,std::vector<int> > pq;
	srand(time(0));
	std::map<int,std::string> m;
	for(int i=0;i<5000;++i){
		if((rand()%10)<6){
			int nextInt= rand();
			if(m.find(nextInt)!=m.end())
				continue;
			std::string nextStr=genstr();
			m[nextInt]=nextStr;
			++expectedSize;
			std::cout<<"lh.push("<<nextInt<<",\""<<nextStr<<"\");\n";
			std::cout<<"assert(lh.size()=="<<expectedSize<<");\n";
			pq.push(nextInt);
			std::cout<<"assert(lh.top()==\""<<  m[pq.top()] <<"\");\n";
		}
		else{
			if(expectedSize==0)
				continue;
			--expectedSize;
			m.erase(pq.top());
			pq.pop();
			std::cout<<"lh.pop();\n";
			std::cout<<"assert(lh.size()=="<<expectedSize<<");\n";
			if(expectedSize){
				std::cout<<"assert(lh.top()==\""<< m[pq.top()] <<"\");\n";
			}
		}
		std::cout<<"validate(lh.root_);\n";
	}
	std::cout<< "}";
}
typedef LeftHeap<int,std::string, std::greater<int> > heap;
void test();
int main() {
	genStressTest();
	//test();
	return 0;
}

void validate(heap::Node* node){
	if(node->l==0)
		assert(node->r==0);
	assert(node->r==0 || node->l==0 || node->r->height <= node->l->height);
	if(node->l){
		validate(node->l);
	}
	if(node->r){
		assert(node->height == node->r->height+1);
		validate(node->r);
	}
}

