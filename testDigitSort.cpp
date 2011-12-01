#include <iostream>
#include <string>
#include <utility>
typedef unsigned long long ULL;
typedef std::pair<ULL,ULL> BigPair;
#include "digit_sort.h"
#include "StringSort.h"
#include "NumberSort.h"
#include "BigPairSortClass.h"

#include "generator.h"

template<typename T, typename F, typename SortClass>
void test(int n, F generator, std::string type, SortClass sorter) {
	std::cout << n << " elements of Type " << type << std::endl;
	std::vector<T> arr(n);
	std::generate(arr.begin(), arr.end(), generator);
	clock_t start = clock();
	std::sort(arr.begin(), arr.end());
	std::cout << "STL: " << ((clock() - start) / 1000) << " ms" << std::endl;

	std::generate(arr.begin(), arr.end(), generator);
	start = clock();
	digit_sort(arr.begin(), arr.end(), sorter);
	std::cout << "My: " << ((clock() - start) / 1000) << " ms" << std::endl;
	if (std::is_sorted(arr.begin(), arr.end())) {
		std::cout << "Sorted correctly";
	} else
		std::cout << "Sorting INCORRECT!";

	std::cout << std::endl << std::endl;
}


void testAll() {
	const int BYTE = 8;
	test<unsigned>(5000000, rand,"int", NumberSort(2, 2 * BYTE));
	test<unsigned>(5000000, rand,"int", NumberSort(4, BYTE));
	test<ULL>(5000000, genLong,"long long", NumberSort(4, 2 * BYTE));
	test<ULL>(5000000, genLong,"long long", NumberSort(8, BYTE));
	test<BigPair> (500000, genPair, "\"pair of long long\"", BigPairSort());
	test<std::string> (30000, genString,"string", StringSort());
}
