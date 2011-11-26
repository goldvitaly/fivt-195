/* 
 * File:   test.h
 * Author: riad
 *
 * Created on September 26, 2011, 3:46 PM
 */
#include <iostream>

#include "digit_sort.h"
#include "StringSort.h"
#include "NumberSort.h"
#include "BigPairSortClass.h"

#include "generator.h"
namespace DigitSort {

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

	void testAll() {
		const int BYTE = 8;
		test<unsigned int>(500000, rand, (char*) "int", NumberSort(2, 2 * BYTE));
		test<unsigned int>(500000, rand, (char*) "int", NumberSort(4, BYTE));
		test<unsigned long long>(50000000, genLong, (char*) "long long", NumberSort(4, 2 * BYTE));
		test<unsigned long long>(50000000, genLong, (char*) "long long", NumberSort(8, BYTE));
		test < std::pair<unsigned long long, unsigned long long >> (500000, gen_pair, (char*) "\"pair of long long\"", BigPairSort());
		test<std::string > (30000, genstring, (char*) "string", StringSort());
		
	}
};