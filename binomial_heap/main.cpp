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
	binomial_heap <T, Comparator> second;
	for (int i = 0; i < second_heap_size; i ++)
	{
		T value = generator();
		second.insert(value);
		all.insert(value);
	}
	binomial_heap <T, Comparator> merged_heap = binomial_heap<T, Comparator>::merge(first, second);
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
int insert_test(size_t size, Generator generator, Comparator comparator = Comparator() )
{
	binomial_heap<T, Comparator> heap(comparator);
	std::multiset<T, Comparator> set(comparator);
	for (int i = 0; i < size; i ++)
	{
		T w = generator();
		set.insert(w);
		heap.insert(w);
		if (*set.begin() != heap.min())
		{
			std::cerr << "Wrong answer on step " << i + 1 <<". Expected minimum: " << *set.begin() << ", got: " << heap.min() << std::cerr;
			return 1;
		}
	}
	int step = 0;
	while (set.size() > 0 && heap.size() > 0)
	{
		T waited_value = heap.min();
		T heap_min_value = heap.extract_min();
		heap.check_heap_property();
		assert(waited_value == heap_min_value, "Waited: " << waited_value << ", got: " << heap_min_value);
		T set_min_value = *set.begin();
		set.erase(set.begin());
		step ++;
		if (heap_min_value != set_min_value)
		{
			std::cerr << "Error: incorrect value extracted on step " << step << ". Expected: " << set_min_value << ", got: " << heap_min_value << std::endl;
			return 1;
		}
	}
	if (set.size() != 0)
	{
		std::cerr << "Error: incorrect size" << std::endl;
		return 1;
	}
	if (heap.size() != 0)
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
