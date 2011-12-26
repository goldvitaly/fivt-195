#include "extsort.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <cassert>

template<typename ValueType, class Comparator>
class STDSorter
{
	public:
		typedef ValueType type;
		typedef Comparator comp;
		
		void operator()(ValueType* begin, ValueType* end) const
		{
			std::sort(begin, end, Comparator());
		}
};

int main()
{
	typedef double input_type;
	typedef std::less<input_type> comparator;
	typedef STDSorter<input_type,comparator> sorter;
	
	std::fstream input;
	input.open("testinput.txt",std::fstream::in);
	std::fstream output;
	output.open("testoutput.txt",std::fstream::out);
	
	externalSort(input, output, sorter());
	
	output.close();
	output.open("testoutput.txt", std::fstream::in);
	std::vector<input_type> v;
	input_type next;
	while(output >> next)
		v.push_back(next);
	std::vector<input_type> vs = v;
	std::sort(vs.begin(), vs.end(), comparator());
	for(unsigned int i = 0; i < v.size(); i++)
		assert(v[i] == vs[i]);
	
	std::cerr << "Passed _ALL_ the tests" << std::endl;
	
	input.close();
	output.close();
	
	return 0;
}
