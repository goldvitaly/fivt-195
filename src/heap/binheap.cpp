#include "binheap.h"
#include <cstdio>

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
		if( nodes[i] != NULL && (ind == -1 || ( nodes[i] -> value > nodes[ind] -> value))){
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
	node* tmp = nodes[ind];
	nodes[ind] = NULL;
	merge(new binheap(tmp));
	tidy();
}

int binheap::top(){    
	int ind = bestIndex();
	assert(ind != -1);
	return nodes[ind] -> value;
}

void binheap::merge(binheap* b){
	int sz=max(nodes.size(),b -> nodes.size()) + 1;
	nodes.resize(sz,NULL);
	b -> nodes.resize(sz,NULL);
	node* carry = NULL;
	for(int i = 0 ; i < sz; i++){
		if(carry && b -> nodes[i])
			carry = node::merge(carry,b -> nodes[i]);
		else if(carry || b -> nodes[i]){
			if(!carry)
				carry = b -> nodes[i];
			if(!nodes[i]){
				nodes[i] = carry;
				carry = NULL;
			}
			else{
				carry = node::merge(carry,nodes[i]);
				nodes[i] = NULL;
			}
		}
	}
	tidy();
}

void binheap::print(){
	for(int i=0;i<nodes.size();i++){
		if(nodes[i])printf("%d ",nodes[i] -> value);
		else printf("- ");
	}
	printf("\n");
}

/*void binheap::tree(){
	
}*/
