#include "general.h"
#include <vector>
#include <cstdlib>

using namespace std;

template<typename T> class node{
	
	private:
		
		node(node* copyFrom);
	
	public:
		
		T value;
		vnode children;
		
		node(T key);
		~node();
		
		static node* merge(node* a,node* b);
		
};

template <typename T> node<T>::node(node* copyFrom){
	value = copyFrom -> value;
	children = copyFrom -> children;
}

template <typename T> node<T>::node(T key){
	value = key;
}

template <typename T> node<T>::~node(){
	children.clear();
}

template <typename T> node<T>* node<T>::merge(node<T>* a,node<T>* b){
	assert(a != NULL && b != NULL);
	node<T>* ret;
	if((a -> value) < (b -> value)){
		ret = new node<T>(b);
		ret -> children.push_back(a);
	}
	else{
		ret = new node<T>(a);
		ret -> children.push_back(b);
	}
	return ret;
}
