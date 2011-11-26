/* 
 * File:   digit_sort.h
 * Author: riad
 *
 * Created on September 26, 2011, 11:35 AM
 */
#include <algorithm>
#include <vector>
namespace DigitSort {
	typedef unsigned SortInt;
	template <typename Iterator, typename SortClass>
	void digit_sort(Iterator begin, Iterator end, SortClass Sort) {
		std::vector < __typeof__(*begin) > tmp(end - begin);
		for (int pos = Sort.getKeyCount(begin, end) - 1; pos >= 0; --pos) {
			SortInt max = Sort.getMax(pos);
			std::vector<size_t> baskets(max + 2);
			//Может быть по хорошему нужно сохранять итераторы, чтобы был не нужен рандом
			//аксес, но я плохо себе представляю стуктуру, кроме массива, где может быть
			//полезен такой сорт
			for (Iterator it = begin; it != end; ++it) {
				++baskets[Sort.getKey(it, pos) + 1];
			}
			for (int i = 1; i <= max; ++i) {
				baskets[i] += baskets[i - 1];
			}
			for (Iterator it = begin; it != end; ++it) {
				SortInt keys = Sort.getKey(it, pos);
				tmp[baskets[keys]++] = *it;
			}
			std::copy(tmp.begin(), tmp.end(), begin);
		}
	}

};
