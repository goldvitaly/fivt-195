#include "general.h"
#include <vector>
#include <cstdlib>

using namespace std;

template<typename T> class node{
	
	public:
		
		T value;
		vector<node<T>* > children;
		
		explicit node(node* copyFrom);
		explicit node(const T& nodeValue);
		
		static node* merge(node* a, node* b);
		
};

template <typename T> node<T>::node(node* copyFrom){
	value = copyFrom->value;
	children = copyFrom->children;
}

template <typename T> node<T>::node(const T& nodeValue){
	value = nodeValue;
}

template <typename T> node<T>* node<T>::merge(node<T>* a, node<T>* b){
	if(a == NULL || b == NULL)
		return a ? a : b;
	node<T>* ret;
	if(a->value < b->value){
		ret = new node<T>(b);
		ret->children.push_back(a);
	}
	else{
		ret = new node<T>(a);
		ret->children.push_back(b);
	}
	return ret;
}
