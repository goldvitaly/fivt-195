#ifndef INTERVAL_TREE_HPP_
#define INTERVAL_TREE_HPP_

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cassert>

template<typename Element, typename Modification,
         typename MergeFunc, typename ModifyFunc, typename PushUpdateFunc>
class IntervalTree {

 public:

  size_t size() const {
    return size_;
  }

 private:

  class Range {
    int begin_, end_;
    Range(int begin = 0, int end = 0): begin_(begin), end_(end) {}

    Range LeftHalf() const {
      return Range(begin_, (begin_ + end_) >> 1);
    }

    Range RightHalf() const {
      return Range(((begin_ + end_) >> 1) + 1, end_);
    }

    bool IntersectWith(Range target) const {
      return !(begin_ > target.end_ || target.begin_ > end_);
    }

    void CutWith(Range target) {
      return Range(max(begin_, target.begin_), min(end_, target.end_));
    }

    size_t length() const {
      return (end_ - begin_ + 1);
    }

    bool operator ==(Range target) const {
      return ((begin_ == target.begin_) && (end_ == target.end_))
    }

  };
  
  class Node {
   
   public:
    void Recalc() {

      key_ = merge_(left_child_ ? GetKey(left_child_) : zero_element_,
                    right_child_ ? GetKey(right_child_) : zero_element_);

    }

    Element GetKey(size_t length) const
    {
      if (modified_)
        return modify_(key_, modification_, length);
    }

    void PushUpdate() {
      
      if (!left_child_ && !right_child_)
        return;
      
      if (modified_) {

        if (left_child_)
          left_child_->AddModification(modification_);

        if (right_child_)
          right_child_->AddModification(modification_);

        modified_ = 0;
      }

    }

    void AddModification(Modification new_modification) {

      if (modified_)
        modification_ = update_(modification_, new_modification);
      else
        modification_ = new_modification;

      modified_ = 1;

    }

   private:

    Element key_;
    Modification modification_;
    bool modified_;
    Node* left_child_;
    Node* right_child_;
   
  };

  Element GetValue(Node* root, Range current, Range target)
  {
    if (!root)
      return zero_element_;

    root->PushUpdate();
    
    if (!target.IntersectWith(current))
      return zero_element_;
    
    target.CutWith(current);
    
    if (current == target)
      return root->GetKey();

    return merge_(GetValue(root->left_child_, current->LeftHalf(), target), 
                  GetValue(root->right_child_, current->RightHalf(), target));

  }

  size_t size_;
  Element zero_element_;

  MergeFunc merge_;
  ModifyFunc modify_;
  PushUpdateFunc update_;

  Node* root_;

};

#endif // INTERVAL_TREE_HPP_
