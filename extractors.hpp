#ifndef EXTRACTORS_HPP_
#define EXTRACTORS_HPP_
#include <iostream>

template<typename Integer>
class IntegerExtractor {
 public:
  const static size_t block_count = 4;
  const static size_t block_length = sizeof(Integer) * 2;
  const static size_t block_modulo = (1ll << block_length) - 1;

  size_t GetKey(Integer X, int block_num) const {
    return (X >> (block_length * block_num)) & block_modulo;
  }
};

template<typename Integer>
Integer RandomInteger() {
  return rand() & ((1ll << (sizeof(Integer) * 8 - 2)) - 1);
}

class StringExtractor {
 public:
  const static size_t block_count = 5;
  const static size_t block_length = 7;

  size_t GetKey(std::string X, int block_num) const {
    return X[block_count - block_num - 1];
  }
};

std::string RandomString() {
  std::string result;
  for(int i = 0; i < 5; i++) {
    result += (rand() & ((1 << 7) - 1));
  }
  return result;
}

class PairExtractor {
 public:
  const static size_t block_count = 2;
  const static size_t block_length = 16;

  size_t GetKey(std::pair<int, int> X, int block_num) const {
    return block_num ? X.first : X.second;
  }
};

std::pair<int, int> RandomPair() {
  return std::make_pair(rand() & ((1 << 16) - 1), rand() & ((1 << 16) - 1));
}

#endif // EXTRACTORS_HPP_
