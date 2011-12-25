#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP 

#include <iostream>

template<typename Element>
struct MinMergeFunctor {
  Element operator() (const Element& e1, const Element& e2) {
    return std::min(e1, e2);
  }
};

template<typename Element>
struct SumMergeFunctor {
  Element operator() (const Element& e1, const Element& e2) {
    return e1 + e2;
  }
};

template<typename Element>
struct AssignModifyFunctor {
  Element operator() (const Element& e1, const Element& e2, size_t length = 0) {
    return e2;
  }
};

template<typename Element>
struct SumModifyFunctor {
  Element operator() (const Element& e1, const Element& e2, size_t length = 0) {
    return e1 + e2;
  }
};

#endif /* FUNCTORS_HPP */
