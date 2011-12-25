#include <iostream>
#include <vector>
#include <map>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include "binomial_heap.hpp"

using namespace std;

const int element_count = 10000;
const int iterations = 1000000;

vector<int> RandomVector(int size) {
  vector<int> result;
  for(int i = 0; i < size; i++)
    result.push_back(rand());
  return result;
}

bool Sorted(const vector<int>& array) {
  for(int i = 0; i < array.size() - 1; i++)
    if (array[i] > array[i + 1])
      return 0;
  return 1;
}

void Print(const vector<int>& array) {
  for(int i = 0; i < array.size(); i++)
    printf("%d ", array[i]);
  printf("\n");
}

void TestMergeMemoryLeaks(int iterations) {
  for(int i = 0; i < iterations; i++) {
    typedef BinomialTree<int> tree;
    typedef std::shared_ptr<BinomialTree<int>> ptree;

    ptree tree1(new tree(rand()));
    ptree tree2(new tree(rand()));
    ptree result = tree::Merge(tree1, tree2);
  }
}

map<int, int> counter;

int main() {
  srand(42);
  double t1 = clock();
  BinomialHeap<int> heap;
  vector<int> array = RandomVector(element_count);
  for(int i = 0; i < array.size(); i++) {
    counter[array[i]]++;
    heap.Insert(array[i]);
  }
  array.clear();
  for(int i = 0; i < element_count; i++) {
    int top_element = heap.MinElement()->key();
    heap.PopMin();
    array.push_back(top_element);
    counter[top_element]--;
    if (counter[top_element] < 0)
      throw logic_error("Heap returned wrong elements");
  }
  if (!Sorted(array))
    throw logic_error("Bad sort order");

  TestMergeMemoryLeaks(iterations);
  double t2 = clock();
  printf("Execution time: %.3lf\n", (t2 - t1) / CLOCKS_PER_SEC);
  return 0;
}
