#include "interval_tree.hpp"
#include "min_interval_tree.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <exception>

using namespace std;

const int MOD = 2;
const int element_count = 3;

vector<int> RandomVector(int element_count) {
  vector<int> result;
  for(int i = 0; i < element_count; i++)
    result.push_back(rand() % MOD);
  return result;
}

void AssignVector(vector<int>& array, int begin, int end, int value) {
  for(int i = begin; i <= end; i++)
    array[i] = value;
}

void PrintVector(vector<int>& array) {
  for(int i = 0; i < array.size(); i++)
    printf("%d ", array[i]);
  printf("\n");
}

void TestRangeGetQueues(int element_count) {
  MinIntervalTree<int> interval_tree(element_count, 1000000000);
  vector<int> array = RandomVector(element_count);
  for(int i = 0; i < element_count; i++)
    interval_tree.Set(i, i, array[i]);
  for(int iteration = 0; iteration < 1000; iteration++) {
    int x = rand() % element_count;
    int y = rand() % element_count;
    if (x > y)
      swap(x, y);
    int result = *min_element(array.begin() + x, array.begin() + y + 1);
    int tree_result = interval_tree.Get(x, y, 0);
    if (result != tree_result) {
      cout << x << " " << y << endl;
      cout << result << " " << tree_result << endl;
      throw logic_error("IntervalTree::Get returned wrong result");
    }
  }
}

void TestRangeSetQueues(int element_count) {
  MinIntervalTree<int> interval_tree(element_count, MOD);
  vector<int> array = RandomVector(element_count);
  for(int i = 0; i < element_count; i++)
    interval_tree.Set(i, i, array[i]);
  for(int iteration = 0; iteration < 1000000; iteration++) {
    int x = rand() % element_count;
    int y = rand() % element_count;
    int value = rand() % MOD;
    if (x > y)
      swap(x, y);
    interval_tree.Set(x, y, value);
    AssignVector(array, x, y, value);

    int x1 = rand() % element_count;
    int y1 = rand() % element_count;
    if (x1 > y1)
      swap(x1, y1);
    int result = *min_element(array.begin() + x1, array.begin() + y1 + 1);
    int tree_result = interval_tree.Get(x1, y1, 0);
    if (result != tree_result) {
      cout << iteration << endl;
      cout << value << endl;
      cout << x << " " << y << endl;
      cout << x1 << " " << y1 << endl;
      cout << result << " " << tree_result << endl;
      PrintVector(array);
      interval_tree.Get(x1, y1, 1);
      for(int i = 0; i < element_count; i++)
        printf("%d ", interval_tree.Get(i, i));
      printf("\n");
      throw logic_error("IntervalTree::Set worked incorrectly");
    }
  }
}

int main()
{
  srand(42);
//  MinIntervalTree<int> interval_tree(3, 10000);
//  interval_tree.Set(0, 0, 0);
//  interval_tree.Set(1, 1, 0);
//  interval_tree.Set(2, 2, 1);
//  cout << interval_tree.Get(0, 2) << endl;
  TestRangeSetQueues(element_count);
  //TestRangeGetQueues(element_count);
  return 0;
}
