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
			//std::cerr << "Sorting..." << std::endl;
			std::sort(begin, end);//, Comparator());
		}
};

int main()
{
	std::fstream input;
	input.open("testinput.txt",std::fstream::in);
	std::fstream output;
	output.open("testoutput.txt",std::fstream::out);
	externalSort(input, output, STDSorter<int, std::less<int> >());
	input.close();
	output.close();
	return 0;
}
