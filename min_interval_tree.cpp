#include "interval_tree.cpp"

template <typename ElementType>//, typename ModifyType, 
          //typename MergeFunc, typename ModifyFunc, typename ModifyUpdateFunc>
class MinIntervalTree
{

  public:

  MinIntervalTree() {}
  explicit MinIntervalTree(size_t size, const ElementType& element_zero, const ElementType& modify_zero, const ElementType& init_element)
  {
    assert(size > 0);
    tree = new IntervalTree<ElementType, ElementType, MinMerge, AddModify, AddModifyUpdate>(size, element_zero, modify_zero, init_element);
  }

  ElementType GetValue(int left_index, int right_index)
  {
    return tree->GetValue(0, Range(0, tree->size()), Range(left_index, right_index));
  }

  void UpdateRange(int left_index, int right_index, ElementType modify)
  {
    tree->UpdateRange(0, Range(0, tree->size()), Range(left_index, right_index), modify);
  }

  private:

  class MinMerge
  {
    public:

    ElementType operator() (ElementType a, ElementType b)
    {
      return std::min(a, b);
    }
  };
  class AddModify
  {
    public:

    ElementType operator() (ElementType a, ElementType b)
    {
      return a + b;
    }
  };
  class AddModifyUpdate
  {
    public:

    ElementType operator() (ElementType a, ElementType b)
    {
      return a + b;
    }
  };

  IntervalTree<ElementType, ElementType, MinMerge, AddModify, AddModifyUpdate> * tree;

};

int main()
{
  MinIntervalTree<int> T(100, 1000000000, 0, 0);
  DB(T.GetValue(0, 10));
  T.UpdateRange(0, 10, 3);
  DB(T.GetValue(0, 10));
  return 0;
}
