#include "general.h"
#include "Node.h"
#include <cstdio>
#include <vector>

using namespace std;

template <typename T> class Binheap{
	
	private:
		
		vector<Node<T>* > nodes;
		size_t heapsize;
		
		explicit Binheap(const T& single);
		explicit Binheap(Node<T>* parent);
		
		size_t bestIndex() const;
		void cutUnused();
	
	public:
		
		Binheap();
		
		void push(const T& value);
		T extractMax();
		const T& top() const;
		void merge(Binheap* b);
		void print() const;
		size_t size() const;
		bool empty() const; 
};

template <typename T> Binheap<T>::Binheap(const T& single){
	Node<T>* n = new Node<T>(single);
	nodes.push_back(n);
	heapsize = 1;
}

template <typename T> Binheap<T>::Binheap(Node<T> * parent){
	int sz = parent->children.size();
	heapsize = 0;
	for(int i = 0; i < sz; i++){
		nodes.push_back(parent->children[i]);
		if(nodes[i])
			heapsize += 1 << i;
	}
}

template <typename T> size_t Binheap<T>::bestIndex() const{
	int sz = nodes.size();
	int ind = -1;
	for(int i = 0; i < sz; i++){
		if( nodes[i] != NULL && (ind == -1 || ( nodes[i]->value > nodes[ind]->value))){
			ind = i;
		}
	}
	return ind;
}

template <typename T> void Binheap<T>::cutUnused(){
	int sz = nodes.size() - 1;
	while(sz >= 0 && nodes[sz] == NULL)
		sz--;
	nodes.resize(sz + 1, NULL);
}

template <typename T> Binheap<T>::Binheap(){
	heapsize = 0;
}

template <typename T> void Binheap<T>::push(const T& value){
	Binheap<T>* tmp = new Binheap<T>(value);
	merge(tmp);
}

template <typename T> T Binheap<T>::extractMax(){
	int ind = bestIndex();
	assert(ind != -1);
	T ret = nodes[ind]->value;
	Node<T>* tmp = nodes[ind];
	nodes[ind] = NULL;
	heapsize -= (1<<ind);
	merge(new Binheap<T>(tmp));
	cutUnused();
	return ret;
}

template <typename T> const T& Binheap<T>::top() const{    
	int ind = bestIndex();
	assert(ind != -1);
	return nodes[ind]->value;
}

template <typename T> void Binheap<T>::merge(Binheap<T>* b){
	int sz = max(nodes.size(), b->nodes.size()) + 1;
	nodes.resize(sz, NULL);
	b->nodes.resize(sz, NULL);
	Node<T>* carry = NULL;
	for(int i = 0 ; i < sz; i++){
		if(carry && b->nodes[i])
			carry = Node<T>::merge(carry, b->nodes[i]);
		else{
			if(!carry)
				carry = b->nodes[i];
			bool needToCarry = carry && nodes[i];
			nodes[i] = Node<T>::merge(carry, nodes[i]);
			carry = NULL;
			if(needToCarry){
				carry = new Node<T>(nodes[i]);
				nodes[i] = NULL;
			}
		}
	}
	cutUnused();
	heapsize += b->size();
}

template <typename T> void Binheap<T>::print() const{
	for(int i=0;i<nodes.size();i++){
		if(nodes[i])
			printf("%d ", nodes[i]->value);
		else
			printf("- ");
	}
	printf("\n");
}

template <typename T> size_t Binheap<T>::size() const{
	return heapsize;
}

template <typename T> bool Binheap<T>::empty() const{
	return size() == 0;
}
