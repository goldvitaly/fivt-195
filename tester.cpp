#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <time.h>
#include <cassert>

#include "extractors.hpp"
#include "digit_sort.hpp"

using namespace std;

template <typename Iterator>
void TestSTDSort(Iterator begin, Iterator end) {
  double t1 = clock();
  sort(begin, end);
  double t2 = clock();
  printf("STL: %.lf\n", (t2 - t1) / 1000.0);
}

template <typename Iterator, typename BlockExtractor>
void TestDigitSort(Iterator begin, Iterator end, const BlockExtractor& BE) {
  double t1 = clock();
  DigitSort(begin, end, BE);
  double t2 = clock();
  printf("Digit: %.lf\n", (t2 - t1) / 1000.0);
}

template <typename Iterator>
void CheckSortResult(Iterator begin1, Iterator end1, Iterator begin2, Iterator end2) {
  while (begin1 != end1) {
    assert(*begin1 == *begin2);
    begin1++;
    begin2++;
  }
  printf("Match!\n");
}

template <typename Element, typename Generator, typename BlockExtractor>
void Test(int n, const Generator& G, const BlockExtractor& BE) {
  vector<Element> T(n), S1(n), S2(n);
  generate(T.begin(), T.end(), G);
  copy(T.begin(), T.end(), S1.begin());
  copy(T.begin(), T.end(), S2.begin());

  TestSTDSort(S1.begin(), S1.end());
  TestDigitSort(S2.begin(), S2.end(), BE);
  CheckSortResult(S1.begin(), S1.end(), S2.begin(), S2.end());

  T.clear(); S1.clear(); S2.clear();
}

const int int_N = 10000000;
const int longlong_N = 10000000;
const int string_N = 100000;
const int pair_N = 10000000;

void TestAll() {
  printf("n = %d, int:\n", int_N);
  Test<int>(int_N, RandomInteger<int>, IntegerExtractor<int>());
  printf("\n");

  printf("n = %d, long long:\n", longlong_N);
  Test<long long>(longlong_N, RandomInteger<long long>, IntegerExtractor<long long>());
  printf("\n");

  printf("n = %d, string:\n", string_N);
  Test<string>(string_N, RandomString, StringExtractor());
  printf("\n");

  printf("n = %d, pair:\n", pair_N);
  Test< pair<int,int> >(pair_N, RandomPair, PairExtractor());
  printf("\n");
}

int main() {
  srand(time(NULL));
  TestAll();
  return 0;
}
