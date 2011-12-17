#include <iostream>
#include <set>
#include "binomial_heap.h"

template <class T, class Generator, class Comparator = std::less<T> > 
int merge_test(int first_heap_size, int second_heap_size, Generator generator, Comparator comparator = Comparator())
{
	std::multiset <T, Comparator> all(comparator);
	binomial_heap <T, Comparator> first(comparator);
	for (int i = 0; i < first_heap_size; i ++)
	{
		T value = generator();
		first.insert(value);
		all.insert(value);
	}
	binomial_heap <T> second;
	for (int i = 0; i < second_heap_size; i ++)
	{
		T value = generator();
		second.insert(value);
		all.insert(value);
	}
	binomial_heap <T> merged_heap = binomial_heap<T>::merge(first, second);
	assert(all.size() == merged_heap.size(), "Incorrect size of merged heap. Should be " << all.size() << " instead of " << merged_heap.size());
	while (merged_heap.size())
	{
		assert(all.size() != 0, "Incorrect minimum extracting. Deleting is too slow: elements have alrealy finished");
		T min_value = merged_heap.extract_min();
		assert(min_value == *all.begin(), "Incorrect extracted element: should be " << *all.begin() << " " << "instead of " << min_value << std::endl);
		all.erase(all.begin());
	}
	assert(all.size() == 0, "Deleting is too quick: there are should be some more elements in heap");
	std::cerr << "OK" << std::endl;
};

template <class T, class Generator, class Comparator = std::less<T> >
int insert_test(int n, Generator generator, Comparator comparator = Comparator() )
{
	binomial_heap<T, Comparator> t(comparator);
	std::multiset<T, Comparator> s(comparator);
	for (int i = 0; i < n; i ++)
	{
		T w = generator();
		s.insert(w);
		t.insert(w);
		if (*s.begin() != t.min())
		{
			std::cerr << "WA!" << std::cerr;
			return 1;
		}
	}
	int step = 0;
	while (s.size() > 0 && t.size() > 0)
	{
		T waited_value = t.min();
		T v = t.extract_min();
		t.check_heap_property();
		assert(waited_value == v, "Waited: " << waited_value << ", got: " << v);
		T u = *s.begin();
		s.erase(s.begin());
		step ++;
		if (v != u)
		{
			std::cerr << "Error: incorrect value extracted on step " << step << ". Expected: " << u << ", got: " << v << std::endl;
			return 1;
		}
	}
	if (s.size() != 0)
	{
		std::cerr << "Error: incorrect size" << std::endl;
		return 1;
	}
	if (t.size() != 0)
	{
		std::cerr << "Error: incorrect size" << std::endl;
		return 1;
	}
	std::cerr << "OK" << std::endl;
	return 0;
};

int main()
{
	insert_test<int>(1000, [](){ return rand() % 100; }, std::greater<int>());
	merge_test<int>(1000,1000,rand);
	return 0;
}
