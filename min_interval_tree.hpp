#ifndef MIN_INTERVAL_TREE_HPP
#define MIN_INTERVAL_TREE_HPP 

#include <iostream>

#include "interval_tree.hpp"
#include "functors.hpp"

template<typename Element>
class MinIntervalTree {

 public:
  MinIntervalTree(size_t size, Element zero_element) {
    tree_ = IntervalTree
    <
      Element, 
      Element, 
      MinMergeFunctor<Element>, 
      AssignModifyFunctor<Element>, 
      AssignModifyFunctor<Element>
    >();
    tree_.Initialize(size, zero_element);
  }

  Element Get(int begin, int end, int type) {
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
    MinMergeFunctor<Element>, 
    AssignModifyFunctor<Element>, 
    AssignModifyFunctor<Element>
  > tree_;
};

#endif /* MIN_INTERVAL_TREE_HPP */
