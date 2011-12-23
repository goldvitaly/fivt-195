#include <iostream>
#include <list>
#include <vector>
#include <cassert>
#include <cstdio>
#include <memory>
#include <algorithm>

#define DB(x) std::cerr << std::endl << #x << " : " << x << std::endl;

class Range
{
  public:

  Range(int l = 0, int r = 0): left_(l), right_(r) {}

  bool operator == (const Range& range) const
  {
    return ((left_ == range.left_) && (right_ == range.right_));
  }

  int middle() const
  {
    return (left_ + right_) / 2;
  }

  Range LeftHalf() const
  {
    return Range(this->left_, this->middle());
  }
  Range RightHalf() const
  {
    return Range(this->middle() + 1, this->right_);
  }

  void BoundWith(Range& bound)
  {
    if (left_ < bound.left_)
      left_ = bound.left_;

    if (right_ > bound.right_)
      right_ = bound.right_;
  }

  bool OutOfRange(const Range& range) const
  {
    return ((left_ > range.right_) || (right_ < range.left_));
  }

  void Print() const
  {
    printf("left = %d; right = %d\n", left_, right_);
  }

  private:

  int left_, right_;
};

template <typename ElementType, typename ModifyType, 
          typename MergeFunc, typename ModifyFunc, typename UpdateModifyFunc>
class IntervalTree
{

  private:

  size_t size_;
  std::vector<ElementType> tree;
  std::vector<bool> updated;
  std::vector<ModifyType> modification;
  MergeFunc merge;
  ModifyFunc modify;
  UpdateModifyFunc update_modify;
  ElementType element_zero_;
  ModifyType modify_zero_;

  public:

  IntervalTree() {}
  explicit IntervalTree(size_t size, const ElementType& element_zero, const ModifyType& modify_zero, const ElementType& init_element)
  {
    assert(size > 0);
    size_ = 1;
    element_zero_ = element_zero;
    modify_zero_ = modify_zero;
    while (size_ < size)
      size_ <<= 1;
    tree.assign(size, init_element);
    updated.assign(size, 0);
    modification.assign(size, modify_zero);
  }

  size_t size() const
  {
    return size_;
  }

  void RecalcNode(int index)
  {
    assert(index < size());
    int l = index*2, r = index*2 + 1;
    tree[index] = merge(modify(tree[l], modification[l]), modify(tree[r], modification[r]));
  }

  void SetModification(int index, const ModifyType& modify)
  {
    modification[index] = update_modify(modification[index], modify);
    updated[index] = 1;
  }

  void PushNodeUpdate(int index)
  {
    assert(index < size());
    if (updated[index])
    {
      int l = index*2, r = index*2 + 1;
      SetModification(l, modification[index]);
      SetModification(r, modification[index]);
      updated[index] = 0;
      modification[r] = modify_zero_;
    }
  }

  void UpdateRange(int root, Range root_range, Range update_range, const ModifyType& modify)
  {
    if (update_range.OutOfRange(root_range))
      return;

    update_range.BoundWith(root_range);

    PushNodeUpdate(root);

    if (root_range == update_range)
    {
      SetModification(root, modify);
      return;
    }

    UpdateRange(root*2,     root_range.LeftHalf(), update_range, modify);
    UpdateRange(root*2 + 1, root_range.RightHalf(), update_range, modify);
    RecalcNode(root);
  }

  ElementType GetValue(int root, Range root_range, Range update_range)
  {
    if (update_range.OutOfRange(root_range))
      return element_zero_;

    update_range.BoundWith(root_range);

    PushNodeUpdate(root);

    if (root_range == update_range)
      return modify(tree[root], modification[root]);

    return merge(
      GetValue(root*2,     root_range.LeftHalf(), update_range),
      GetValue(root*2 + 1, root_range.RightHalf(), update_range)
    );
  }

};

/*int main()
{
  //freopen("input.txt", "r", stdin);
  return 0;
}*/
