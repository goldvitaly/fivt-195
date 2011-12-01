#include <iostream>

#include "digit_sort.h"
#include "StringSort.h"
#include "NumberSort.h"
#include "BigPairSortClass.h"

#include "generator.h"

template<typename T, typename F, typename SortClass>
void test(int n, F func, char * type, SortClass Sort) {
	std::cout << n << " elements of Type " << type << std::endl;
	T* arr = new T[n];
	std::generate(arr, arr + n, func);
	clock_t start = clock();
	std::sort(arr, arr + n);
	std::cout << "STL: " << ((clock() - start) / 1000) << " ms" << std::endl;

	std::generate(arr, arr + n, func);
	start = clock();
	digit_sort(arr, arr + n, Sort);
	std::cout << "My: " << ((clock() - start) / 1000) << " ms" << std::endl;
	if (std::is_sorted(arr, arr + n)) {
		std::cout << "Sorted correctly";
	} else
		std::cout << "Sorting INCORRECT!";

	std::cout << std::endl << std::endl;

	delete[] arr;
}

typedef unsigned long long ULL;
void testAll() {
	const int BYTE = 8;
	test<unsigned>(500000, rand, (char*) "int", NumberSort(2, 2 * BYTE));
	test<unsigned>(500000, rand, (char*) "int", NumberSort(4, BYTE));
	test<ULL>(50000000, genLong, (char*) "long long", NumberSort(4, 2 * BYTE));
	test<ULL>(50000000, genLong, (char*) "long long", NumberSort(8, BYTE));
	test <std::pair<ULL,ULL>> (500000, genPair, (char*) "\"pair of long long\"", BigPairSort());
	test<std::string> (30000, genString, (char*) "string", StringSort());

}
