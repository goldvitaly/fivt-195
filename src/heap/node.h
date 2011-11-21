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
