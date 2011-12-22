#include <iostream>
#include <list>
#include <vector>
#include <cassert>
#include <cstdio>
#include <memory>
#include <algorithm>

#define DB(x) std::cerr << std::endl << #x << " : " << x << std::endl;

template<typename Key>
class BinomialTree
{

  public:
  typedef BinomialTree<Key> tree;
  typedef std::shared_ptr<tree> ptree;

  BinomialTree() {}
  explicit BinomialTree(const Key& k)
  {
    key_ = k;
    rank_ = 0;
    size_ = 1;
  }

  int size() const
  {
    return size_;
  }
  int rank() const
  {
    return rank_;
  }
  int key() const
  {
    return key_;
  }

  static std::list<ptree> split(ptree target)
  {
    std::list<ptree> T;
    if (target)
    {
      T = target->child_;
      target = 0;
    }
    return T;
  }

  static ptree merge(ptree A, ptree B)
  {
    if (!A)
      return B;
    if (!B)
      return A;

    assert(A->rank() == B->rank());

    if (A->key() > B->key())
      std::swap(A, B);

    ptree T(new tree(A->key()));
    T->rank_ = A->rank_ + 1;

    T->child_.push_back(B);
    T->child_.splice(T->child_.begin(), A->child_);

    T->size_ = A->size_ + B->size_;

    A = B = 0;
    return T;
  }

  private:

  Key key_;
  int size_, rank_;
  std::list<ptree> child_;

};

template<typename Key>
class BinomialHeap
{

  public:

  typedef BinomialTree<Key> tree;
  typedef std::shared_ptr<tree> ptree;
  typedef std::shared_ptr<BinomialHeap<Key>> pheap;
  typedef typename std::list<ptree>::iterator plist;

  int size() const
  {
    return size_;
  }

  void addToRootList(ptree& item)
  {
    if (item)
    {
      this->root_list_.push_back(item);
      this->size_ += item->size();
      item = 0;
    }
  }

  void merge(pheap H)
  {
    int maxrank = 0;
    
    for(auto it : H->root_list_)
      maxrank = std::max(maxrank, it->rank());

    for(auto it : root_list_)
      maxrank = std::max(maxrank, it->rank());
    
    std::vector<ptree> add_items1(maxrank + 2), add_items2(maxrank + 2);

    for(auto it : H->root_list_)
      add_items1[it->rank()] = it;

    for(auto it : root_list_)
      add_items2[it->rank()] = it;
      
    this->size_ = 0;
    this->root_list_.clear();

    ptree carry = 0;
    for(int i=0; i <= maxrank + 1; ++i)
    {
      if (carry && add_items1[i] && add_items2[i])
        addToRootList(carry);

      carry = tree::merge(add_items1[i], carry);
      carry = tree::merge(add_items2[i], carry);

      if (carry && (carry->rank() == i))
        addToRootList(carry);
    }
    H = 0;
  }
  
  void insert(const Key &k)
  {
    pheap T(new BinomialHeap<Key>);
    T->root_list_.push_back(ptree(new tree(k)));
    this->merge(T);
  }

  const plist findMinIter()
  {
    return min_element(root_list_.begin(), root_list_.end(), [](ptree a, ptree b) {
                                                              return (a->key() < b->key()); 
    });
  }

  const ptree findMin()
  {
    return *findMinIter();
  }

  void popMin()
  {
    plist pMin = findMinIter();
    pheap H (new BinomialHeap<Key>);
    H->root_list_ = tree::split(*pMin);
    root_list_.erase(pMin);
    this->merge(H);
  }

  private:

  std::list<ptree> root_list_;
  int size_;

};

int main()
{
  freopen("input.txt", "r", stdin);
  BinomialHeap<int> H;
  std::shared_ptr<BinomialTree<int>> Pnt;
  int n;
  scanf("%d", &n);
  for(int i=0; i<n; i++)
  {
    int prior;
    scanf("%d", &prior);
    H.insert(prior);
  }
  for(int i=0; i<n; i++)
  {
    if (i) printf(" ");
    Pnt = H.findMin();
    printf("%d", Pnt->key());
    H.popMin();
  }
  printf("\n");
  return 0;
}
