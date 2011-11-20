#include "general.h"
#include <vector>
#include <cstdlib>

using namespace std;

class node{
	
	private:
		
		node(node* copyFrom);
	
	public:
		
		int value;
		vnode children;
		
		node(int key);
		~node();
		
		static node* merge(node* a,node* b);
		
};
