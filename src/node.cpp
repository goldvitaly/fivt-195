#include "node.h"

node::node(int key){
	value = key;
}

node::~node(){
	children.clear();
}

node* merge(node* a,node* b){
	assert(a != NULL && b != NULL);
	if((a -> value) < (b -> value))
		swap(a,b);
	a -> children.push_back(b);
	return a;
}
