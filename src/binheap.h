#include "general.h"
#include "node.h"
#include <vector>

using namespace std;

class binheap{
	
	private:
		
		vnode nodes;
		
		binheap(int single);
		binheap(node* parent);
		
		int bestIndex();
		void tidy();
	
	public:
		
		binheap();
		~binheap();
		
		void push(int value);
		void pop();
		int top();
		void merge(binheap* b);
	
};
