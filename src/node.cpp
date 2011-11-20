#include "node.h"

node::node(node* copyFrom){
	value = copyFrom -> value;
	children = copyFrom -> children;
}

node::node(int key){
	value = key;
}

node::~node(){
	children.clear();
}

node* node::merge(node* a,node* b){
	assert(a != NULL && b != NULL);
	node* ret;
	if((a -> value) < (b -> value)){
		ret = new node(b);
		ret -> children.push_back(a);
	}
	else{
		ret = new node(a);
		ret -> children.push_back(b);
	}
	return ret;
}
