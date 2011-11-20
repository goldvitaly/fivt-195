#include "general.h"
#include <vector>
#include <cstdlib>

using namespace std;

class node{
	
	private:
		
		
	
	public:
		
		int value;
		vnode children;
		
		node(int key);
		~node();
		
		void merge(node* n);
		
};
