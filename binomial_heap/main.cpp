#include <iostream>
#include <set>
#include "binomial_heap.h"

int merge_test(int first_heap_size, int second_heap_size, int module)
{
	std::cerr << "Running merge test. Heap sizes are " << first_heap_size << " and " << second_heap_size << ". Maxvalue is " << module - 1 << std::endl;
	std::multiset <int> all;
	binomial_heap <int> first;
	for (int i = 0; i < first_heap_size; i ++)
	{
		int number = rand() % module;
		first.insert(number);
		all.insert(number);
	}
	binomial_heap <int> second;
	for (int i = 0; i < second_heap_size; i ++)
	{
		int number = rand() % module;
		second.insert(number);
		all.insert(number);
	}
	binomial_heap <int> merged_heap = binomial_heap<int>::merge(first, second);
	assert(all.size() == merged_heap.size(), "Incorrect size of merged heap. Should be " << all.size() << " instead of " << merged_heap.size());
	while (merged_heap.size())
	{
		assert(all.size() != 0, "Incorrect minimum extracting. Deleting is too slow: elements have alrealy finished");
		int min_value = merged_heap.extract_min();
		assert(min_value == *all.begin(), "Incorrect extracted element: should be " << *all.begin() << " " << "instead of " << min_value << std::endl);
		all.erase(all.begin());
	}
	assert(all.size() == 0, "Deleting is too quick: there are should be some more elements in heap");
	std::cerr << "OK" << std::endl;
};

int insert_test(int n, int module)
{
	std::cerr << "Running insert test. Heap size is " << n << std::endl;
	binomial_heap<int> t;
	std::set<int> s;
	for (int i = 0; i < n; i ++)
	{
		int w = rand() % module;
		s.insert(w);
		t.insert(w);
		if (*s.begin() != t.min())
		{
			std::cerr << "WA!" << std::cerr;
			return 1;
		}
	}
	std::cerr << "OK" << std::endl;
	return 0;
};

int main()
{
	insert_test(1e3,1e3);
	merge_test(1e3,1e3,1e3);
	merge_test(1e2,1e3,1e3);
	return 0;
}
