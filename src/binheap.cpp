#include "binheap.h"

binheap::binheap(int single){
	node* n = new node(single);
	nodes.push_back(n);
}

binheap::binheap(node* parent){
	int sz = parent -> children.size();
	for(int i = 0; i < sz; i++){
		nodes.push_back(parent -> children[i]);
	}
}

int binheap::bestIndex(){
	int sz = nodes.size();
	int ind = -1;
	for(int i = 0; i < sz; i++){
		if(ind == -1 || (nodes[i] != NULL && nodes[i] -> value > nodes[ind] -> value)){
			ind = i;
		}
	}
	return ind;
}

void binheap::tidy(){
	int sz = nodes.size() - 1;
	while(sz >= 0 && nodes[sz]==NULL)
		sz--;
	nodes.resize(sz + 1);
}

binheap::binheap(){}

binheap::~binheap(){
	nodes.clear();
}

void binheap::push(int value){
	binheap* tmp = new binheap(value);
	merge(tmp);
}

void binheap::pop(){
	int ind = bestIndex();
	assert(ind != -1);
	nodes[ind] = NULL;
	tidy();
}

int binheap::top(){
	int ind = bestIndex();
	assert(ind != -1);
	return nodes[ind] -> value;
}

void binheap::merge(binheap* b){
	
}
