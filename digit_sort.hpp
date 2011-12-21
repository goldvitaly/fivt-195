#ifndef DIGIT_SORT_HPP_
#define DIGIT_SORT_HPP_

#include <iostream>
#include <algorithm>
#include <vector>

template<typename Iterator, typename BlockExtractor>
void DigitSort(const Iterator& begin, const Iterator& end, const BlockExtractor& BE) {

  size_t size = (1 << BE.block_length) + 1;
  std::vector<typename std::iterator_traits<Iterator>::value_type> sort_storage(end - begin);
  std::vector<int> count(size, 0);

  for(int i = 0; i < BE.block_count; i++) {

    for(Iterator it = begin; it != end; ++it) {
      count[BE.GetKey(*it, i) + 1]++;
    }

    for(int it = 1; it < size; ++it) {
      count[it] += count[it-1];
    }

    for(Iterator it = begin; it != end; ++it) {
      sort_storage[count[BE.GetKey(*it, i)]++] = *it;
    }

    copy(sort_storage.begin(), sort_storage.end(), begin);
    count.assign(size, 0);
  }
}

#endif // DIGIT_SORT_HPP_
