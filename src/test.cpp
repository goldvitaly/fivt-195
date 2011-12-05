#include "extsort.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>

template<typename Type, class Comparator>
class STDSorter
{
	public:
		typedef Type type;
		typedef Comparator comp;
		
		void operator()(Type* begin, Type* end)
		{
			std::sort(begin, end, Comparator());
		}
};

int main()
{
	std::fstream input;
	input.open("testinput.txt",std::fstream::in);
	std::fstream output;
	output.open("testoutput.txt",std::fstream::out);
	externalSort(input, output, STDSorter<int, std::less<int> >(), 10);
	input.close();
	output.close();
	return 0;
}
