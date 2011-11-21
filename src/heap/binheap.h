#include "general.h"
#include "node.h"
#include <vector>

using namespace std;

template <typename T> class binheap{
	
	private:
		
		vnode nodes;
		
		binheap(T single);
		binheap(node<T>* parent);
		
		int bestIndex();
		void tidy();
	
	public:
		
		binheap();
		~binheap();
		
		void push(T value);
		void pop();
		T top();
		void merge(binheap* b);
		void print();
		//void tree();
};

