#include "interval_tree.hpp"
#include "min_interval_tree.hpp"

#include <iostream>

using namespace std;

int main()
{
  MinIntervalTree<int> interval_tree(4, 1000000000);
  interval_tree.Set(1, 2, 5);
  //cout << interval_tree.Get(1, 1) << endl;
  return 0;
}
