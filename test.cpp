#include "interval.h"

int main()
{
	IntervalTree<double>* itree = new IntervalTree<double>(10);
	//Tests here...
	delete itree;
	
	return 0;
}
