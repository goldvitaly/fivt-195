#include <iostream>
#include <list>
#include <vector>
#include <cassert>
#include <cstdio>
#include <memory>
#include <algorithm>

#define DB(x) std::cerr << std::endl << #x << " : " << x << std::endl;

template<typename Key>
<<<<<<< HEAD
class BinomialTree
{

  public:
  typedef BinomialTree<Key> tree;
  typedef std::shared_ptr<tree> ptree;

  BinomialTree() {}
  explicit BinomialTree(const Key& k)
  {
    key_ = k;
=======
class binomial_tree
{

  private:

  int size_, rank_;

  public:
  typedef binomial_tree<Key> tree;
  typedef std::shared_ptr<tree> ptree;

  Key key;
  std::list<ptree> Child;

  binomial_tree() {}
  binomial_tree(Key k)
  {
    key = k;
>>>>>>> e9d45f0b728499cddf98b13d91f27a296999bea5
    rank_ = 0;
    size_ = 1;
  }

<<<<<<< HEAD
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
=======
  int size()
  {
    return size_;
  }
  int rank()
  {
    return rank_;
  }

  static std::list<ptree> split(ptree target)
  {
    typename std::list<ptree> T;
    if (target)
    {
      T = target->Child;
>>>>>>> e9d45f0b728499cddf98b13d91f27a296999bea5
      target = 0;
    }
    return T;
  }

  static ptree merge(ptree A, ptree B)
  {
<<<<<<< HEAD
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
=======
    if (!A || !B)
      return A ? A : B;

    assert(A->rank() == B->rank());

    if (A->key > B->key)
      std::swap(A, B);

    ptree T(new tree(A->key));
    T->rank_ = A->rank_ + 1;

    T->Child.push_back(B);
    T->Child.splice(T->Child.begin(), A->Child);

    T->size_ = A->size_ + B->size_;

    A = 0;
    B = 0;
    return T;
  }

};

template<typename Key>
class binomial_heap
{
  private:

  typename std::list< std::shared_ptr<binomial_tree<Key>> > L;
  int size_;

  public:

  typedef binomial_tree<Key> tree;
  typedef std::shared_ptr<tree> ptree;
  typedef std::shared_ptr<binomial_heap<Key>> pheap;
  typedef typename std::list<ptree>::iterator plist;

  int size()
>>>>>>> e9d45f0b728499cddf98b13d91f27a296999bea5
  {
    return size_;
  }

<<<<<<< HEAD
  void addToRootList(ptree& item)
  {
    if (item)
    {
      this->root_list_.push_back(item);
      this->size_ += item->size();
      item = 0;
    }
  }
=======
  /*static bool rank_comp(ptree a, ptree b)
  {
    return (a->rank() < b->rank());
  }*/
>>>>>>> e9d45f0b728499cddf98b13d91f27a296999bea5

  void merge(pheap H)
  {
    int maxrank = 0;
    
<<<<<<< HEAD
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
=======
    for(auto it : H->L)
      maxrank = std::max(maxrank, it->rank());

    for(auto it : L)
      maxrank = std::max(maxrank, it->rank());

    /*if (!H->L.empty())
      maxrank = std::max(maxrank, (*max_element(H->L.begin(), H->L.end(), rank_comp))->rank());

    if (!L.empty())
      maxrank = std::max(maxrank, (*max_element(L.begin(), L.end(), rank_comp))->rank());*/
    
    std::vector<ptree> D1(maxrank + 2), D2(maxrank + 2);

    for(auto it : H->L)
      D1[it->rank()] = it;

    for(auto it : L)
      D2[it->rank()] = it;
      
    this->size_ = 0;
    this->L.clear();
>>>>>>> e9d45f0b728499cddf98b13d91f27a296999bea5

    ptree carry = 0;
    for(int i=0; i <= maxrank + 1; ++i)
    {
<<<<<<< HEAD
      if (carry && add_items1[i] && add_items2[i])
        addToRootList(carry);

      carry = tree::merge(add_items1[i], carry);
      carry = tree::merge(add_items2[i], carry);

      if (carry && (carry->rank() == i))
        addToRootList(carry);
=======
      if (carry && D1[i] && D2[i])
      {
        this->L.push_back(carry);
        this->size_ += carry->size();
        carry = 0;
      }

      carry = tree::merge(D1[i], carry);
      carry = tree::merge(D2[i], carry);

      if (carry && (carry->rank() == i))
      {
        this->L.push_back(carry);
        this->size_ += carry->size();
        carry = 0;
      }
>>>>>>> e9d45f0b728499cddf98b13d91f27a296999bea5
    }
    H = 0;
  }
  
<<<<<<< HEAD
  void insert(const Key &k)
  {
    pheap T(new BinomialHeap<Key>);
    T->root_list_.push_back(ptree(new tree(k)));
=======
  void insert(Key k)
  {
    pheap T(new binomial_heap<Key>);
    T->L.push_back(ptree(new tree(k)));
>>>>>>> e9d45f0b728499cddf98b13d91f27a296999bea5
    this->merge(T);
  }

  plist findMinIter()
  {
<<<<<<< HEAD
    return min_element(root_list_.begin(), root_list_.end(), [](ptree a, ptree b) {
                                                              return (a->key() < b->key()); 
    });
=======
    return min_element(L.begin(), L.end(), [](ptree a, ptree b) { return (a->key < b->key); });
>>>>>>> e9d45f0b728499cddf98b13d91f27a296999bea5
  }

  ptree findMin()
  {
    return *findMinIter();
  }

  void popMin()
  {
    plist pMin = findMinIter();
<<<<<<< HEAD
    pheap H (new BinomialHeap<Key>);
    H->root_list_ = tree::split(*pMin);
    root_list_.erase(pMin);
    this->merge(H);
  }

  private:

  std::list<ptree> root_list_;
  int size_;

=======
    pheap H (new binomial_heap<Key>);
    H->L = tree::split(*pMin);
    L.erase(pMin);
    this->merge(H);
  }

>>>>>>> e9d45f0b728499cddf98b13d91f27a296999bea5
};

int main()
{
  freopen("input.txt", "r", stdin);
<<<<<<< HEAD
  BinomialHeap<int> H;
  std::shared_ptr<BinomialTree<int>> Pnt;
=======
  binomial_heap<int> H;
  std::shared_ptr<binomial_tree<int>> Pnt;
>>>>>>> e9d45f0b728499cddf98b13d91f27a296999bea5
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
<<<<<<< HEAD
    printf("%d", Pnt->key());
=======
    printf("%d", Pnt->key);
>>>>>>> e9d45f0b728499cddf98b13d91f27a296999bea5
    H.popMin();
  }
  printf("\n");
  return 0;
}
