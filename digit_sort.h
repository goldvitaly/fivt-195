#ifndef DIGIT_SORT_H
#define DIGIT_SORT_H
#include <algorithm>
#include <vector>
template <typename Iterator, typename Sorter>
void digit_sort(Iterator begin, Iterator end, Sorter sorter) {
	std::vector<typename std::iterator_traits<Iterator>::value_type> partialSorted(end - begin);
	for (int pos = sorter.getKeyCount(begin, end) - 1; pos >= 0; --pos) {
		int max = sorter.getMaxKey(pos);
		std::vector<size_t> baskets(max + 2);
		//Может быть по хорошему нужно сохранять итераторы, чтобы был не нужен рандом
		//аксес, но я плохо себе представляю стуктуру, кроме массива, где может быть
		//полезен такой сорт
		for (Iterator it = begin; it != end; ++it) {
			++baskets[sorter.getKey(*it, pos) + 1];
		}
		for (int i = 1; i <= max; ++i) {
			baskets[i] += baskets[i - 1];
		}
		for (Iterator it = begin; it != end; ++it) {
			partialSorted[baskets[sorter.getKey(*it, pos)]++] = *it;
		}
		std::copy(partialSorted.begin(), partialSorted.end(), begin);
	}
}
#endif