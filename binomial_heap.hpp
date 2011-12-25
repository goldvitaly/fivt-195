#include <iostream>
#include <list>
#include <vector>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <algorithm>

#define DB(x) std::cerr << std::endl << #x << " : " << x << std::endl;

template<typename Key>
class BinomialTree {

 public:
  typedef BinomialTree<Key> tree;
  typedef std::shared_ptr<tree> ptree;

  BinomialTree() {}
  explicit BinomialTree(const Key& key): key_(key), rank_(0), size_(1) {};

  int size() const {
    return size_;
  }
  int rank() const {
    return rank_;
  }
  int key() const {
    return key_;
  }

  static std::list<ptree> Split(ptree target) {
    std::list<ptree> result;
    if (target) {
      result = target->child_;
      target = 0;
    }
    return result;
  }

  static ptree Merge(ptree A, ptree B) {
    if (!A)
      return B;
    if (!B)
      return A;

    if (A->rank() != B->rank())
      throw std::logic_error("Merging trees with different ranks");

    if (A->key() > B->key())
      std::swap(A, B);

    ptree T(new tree(A->key()));
    T->rank_ = A->rank_ + 1;

    T->child_.push_back(B);
    T->child_.splice(T->child_.begin(), A->child_);

    T->size_ = A->size_ + B->size_;

    A = 0;
    B = 0;
    return T;
  }

 private:
  Key key_;
  int size_, rank_;
  std::list<ptree> child_;
};

template<typename Key>
class BinomialHeap {

 public:
  typedef BinomialTree<Key> tree;
  typedef std::shared_ptr<tree> ptree;
  typedef std::shared_ptr<BinomialHeap<Key>> pheap;
  typedef typename std::list<ptree>::iterator list_iter;

  int size() const {
    return size_;
  }

  void MoveToRootList(ptree& item) {
    if (item) {
      this->root_list_.push_back(item);
      this->size_ += item->size();
      item = 0;
    }
  }

  void Merge(pheap H) {
    int maxrank = 0;
    
    for(auto it : H->root_list_)
      maxrank = std::max(maxrank, it->rank());

    for(auto it : root_list_)
      maxrank = std::max(maxrank, it->rank());
    
    std::vector<ptree> heap1_tree(maxrank + 2), heap2_tree(maxrank + 2);

    for(auto it : H->root_list_)
      heap1_tree[it->rank()] = it;

    for(auto it : root_list_)
      heap2_tree[it->rank()] = it;
      
    this->size_ = 0;
    this->root_list_.clear();

    ptree carry = 0;
    for(int i=0; i <= maxrank + 1; ++i) {
      if (carry && heap1_tree[i] && heap2_tree[i])
        MoveToRootList(carry);

      carry = tree::Merge(heap1_tree[i], carry);
      carry = tree::Merge(heap2_tree[i], carry);

      if (carry && (carry->rank() == i))
        MoveToRootList(carry);
    }
    H = 0;
  }
  
  void Insert(const Key& k) {
    pheap T(new BinomialHeap<Key>);
    T->root_list_.push_back(ptree(new tree(k)));
    this->Merge(T);
  }

  list_iter MinElementIterator() {
    return min_element(root_list_.begin(), root_list_.end(), 
                       [](ptree a, ptree b) { return (a->key() < b->key()); });
  }

  ptree MinElement() {
    return *MinElementIterator();
  }

  void PopMin()
  {
    list_iter min_element_iterator = MinElementIterator();
    pheap H(new BinomialHeap<Key>);
    H->root_list_ = tree::Split(*min_element_iterator);
    root_list_.erase(min_element_iterator);
    this->Merge(H);
  }

 private:
  std::list<ptree> root_list_;
  int size_;
};

