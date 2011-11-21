#include "binheap.h"
#include <cstdio>

template <typename T> binheap<T>::binheap(T single){
	node<T>* n = new node<T>(single);
	nodes.push_back(n);
}

template <typename T> binheap<T>::binheap(node<T> * parent){
	int sz = parent -> children.size();
	for(int i = 0; i < sz; i++){
		nodes.push_back(parent -> children[i]);
	}
}

template <typename T> int binheap<T>::bestIndex(){
	int sz = nodes.size();
	int ind = -1;
	for(int i = 0; i < sz; i++){
		if( nodes[i] != NULL && (ind == -1 || ( nodes[i] -> value > nodes[ind] -> value))){
			ind = i;
		}
	}
	return ind;
}

template <typename T> void binheap<T>::tidy(){
	int sz = nodes.size() - 1;
	while(sz >= 0 && nodes[sz]==NULL)
		sz--;
	nodes.resize(sz + 1);
}

template <typename T> binheap<T>::binheap(){}

template <typename T> binheap<T>::~binheap(){
	nodes.clear();
}

template <typename T> void binheap<T>::push(T value){
	binheap<T>* tmp = new binheap<T>(value);
	merge(tmp);
}

template <typename T> void binheap<T>::pop(){
	int ind = bestIndex();
	assert(ind != -1);
	node<T>* tmp = nodes[ind];
	nodes[ind] = NULL;
	merge(new binheap<T>(tmp));
	tidy();
}

template <typename T> T binheap<T>::top(){    
	int ind = bestIndex();
	assert(ind != -1);
	return nodes[ind] -> value;
}

template <typename T> void binheap<T>::merge(binheap<T>* b){
	int sz=max(nodes.size(),b -> nodes.size()) + 1;
	nodes.resize(sz,NULL);
	b -> nodes.resize(sz,NULL);
	node<T>* carry = NULL;
	for(int i = 0 ; i < sz; i++){
		if(carry && b -> nodes[i])
			carry = node<T>::merge(carry,b -> nodes[i]);
		else if(carry || b -> nodes[i]){
			if(!carry)
				carry = b -> nodes[i];
			if(!nodes[i]){
				nodes[i] = carry;
				carry = NULL;
			}
			else{
				carry = node<T>::merge(carry,nodes[i]);
				nodes[i] = NULL;
			}
		}
	}
	tidy();
}

template <typename T> void binheap<T>::print(){
	for(int i=0;i<nodes.size();i++){
		if(nodes[i])printf("%d ",nodes[i] -> value);
		else printf("- ");
	}
	printf("\n");
}

/*void binheap::tree(){
	
}*/
