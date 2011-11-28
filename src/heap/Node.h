#include "general.h"
#include <vector>
#include <cstdlib>

using namespace std;

template<typename T> class Node{
	
	public:
		
		T value;
		vector<Node<T>* > children;
		
		explicit Node(Node* copyFrom);
		explicit Node(const T& nodeValue);
		
		static Node* merge(Node* a, Node* b);
		
};

template <typename T> Node<T>::Node(Node* copyFrom){
	value = copyFrom->value;
	children = copyFrom->children;
}

template <typename T> Node<T>::Node(const T& nodeValue){
	value = nodeValue;
}

template <typename T> Node<T>* Node<T>::merge(Node<T>* a, Node<T>* b){
	if(a == NULL || b == NULL)
		return a ? a : b;
	Node<T>* ret;
	if(a->value < b->value){
		ret = new Node<T>(b);
		ret->children.push_back(a);
	}
	else{
		ret = new Node<T>(a);
		ret->children.push_back(b);
	}
	return ret;
}
