#include "extsort.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>

template<typename Type, class Comparator>
class STDSorter
{
	public:
		void operator()(Type* begin, Type* end)
		{
			std::sort(begin, end, Comparator());
		}
};

int main()
{
	std::ifstream input;
	input.open("testinput.txt",std::ifstream::in);
	std::ofstream output;
	output.open("testoutput.txt",std::ofstream::out);
	externalSort<int> (input, output, STDSorter<int, std::less<int> >());
	input.close();
	output.close();
	return 0;
}
