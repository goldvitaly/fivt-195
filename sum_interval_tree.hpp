#ifndef SUM_INTERVAL_TREE_HPP
#define SUM_INTERVAL_TREE_HPP 

#include <iostream>

#include "interval_tree.hpp"
#include "functors.hpp"

template<typename Element>
class SumIntervalTree {

 public:
  SumIntervalTree(size_t size, Element zero_element) {
    tree_ = IntervalTree
    <
      Element, 
      Element, 
      SumMergeFunctor<Element>, 
      AssignModifyFunctor<Element>, 
      AssignModifyFunctor<Element>
    >();
    tree_.Initialize(size, zero_element);
  }

  Element Get(int begin, int end) {
    return tree_.GetValue(begin, end);
  }
  void Set(int begin, int end, Element modification) {
    tree_.SetValue(begin, end, modification);
  }

 private:
  IntervalTree
  <
    Element, 
    Element, 
    SumMergeFunctor<Element>, 
    AssignModifyFunctor<Element>, 
    AssignModifyFunctor<Element>
  > tree_;
};

#endif /* SUM_INTERVAL_TREE_HPP */
