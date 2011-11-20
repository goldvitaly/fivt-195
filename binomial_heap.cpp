#include <iostream>
#include <list>
#include <vector>
#include <cassert>
#include <cstdio>
#include <memory>
#include <algorithm>

#define DB(x) std::cerr << std::endl << #x << " : " << x << std::endl;

template<typename Key>
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
    rank_ = 0;
    size_ = 1;
  }

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
      target = 0;
    }
    return T;
  }

  static ptree merge(ptree A, ptree B)
  {
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
  {
    return size_;
  }

  /*static bool rank_comp(ptree a, ptree b)
  {
    return (a->rank() < b->rank());
  }*/

  void merge(pheap H)
  {
    int maxrank = 0;
    
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

    ptree carry = 0;
    for(int i=0; i <= maxrank + 1; ++i)
    {
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
    }
    H = 0;
  }
  
  void insert(Key k)
  {
    pheap T(new binomial_heap<Key>);
    T->L.push_back(ptree(new tree(k)));
    this->merge(T);
  }

  plist findMinIter()
  {
    return min_element(L.begin(), L.end(), [](ptree a, ptree b) { return (a->key < b->key); });
  }

  ptree findMin()
  {
    return *findMinIter();
  }

  void popMin()
  {
    plist pMin = findMinIter();
    pheap H (new binomial_heap<Key>);
    H->L = tree::split(*pMin);
    L.erase(pMin);
    this->merge(H);
  }

};

int main()
{
  freopen("input.txt", "r", stdin);
  binomial_heap<int> H;
  std::shared_ptr<binomial_tree<int>> Pnt;
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
    printf("%d", Pnt->key);
    H.popMin();
  }
  printf("\n");
  return 0;
}
