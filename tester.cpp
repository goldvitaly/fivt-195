#include <iostream>
#include <vector>
#include <map>
#include <exception>
#include <stdexcept>
#include "binomial_heap.hpp"

using namespace std;

const int element_count = 10000;

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

map<int, int> counter;

int main() {
  srand(42);
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
  printf("OK\n");
  return 0;
}
