#ifndef INTERVAL_TREE_HPP_
#define INTERVAL_TREE_HPP_

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cassert>

#define DB(x) std::cerr << #x << " : " << x << std::endl;

template<typename Element, typename Modification,
         typename MergeFunc, typename ModifyFunc, typename PushUpdateFunc>
class IntervalTree {
 
 private:
  class Node;
  class Range;

 public:

  IntervalTree() {
    zero_element_ = Element();
    size_ = 0;
    root_ = NULL;
  }

  void Initialize(size_t size, const Element& zero_element) {
    RemoveTree(root_);
    CreateTree(size, zero_element);
  }

  size_t size() const {
    return size_;
  }

  Element GetValue(int begin, int end, int type) {
    return GetValue(root_, Range(0, size() - 1), Range(begin, end), type);
  }

  void SetValue(int begin, int end, const Modification& modification) {
    SetValue(root_, Range(0, size() - 1), Range(begin, end), modification);
  }

 private:

  struct Range {
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

    Range CutWith(Range target) {
      begin_ = max(begin_, target.begin_);
      end_ = min(end_, target.end_);
    }

    size_t length() const {
      return (end_ - begin_ + 1);
    }

    void Print() const {
      std::cout << begin_ << ":" << end_ << std::endl; 
    }

    bool operator ==(Range target) const {
      return ((begin_ == target.begin_) && (end_ == target.end_));
    }
  }; // Range
  
  class Node {
   
   public:
    explicit Node(const Element& key = Element()) {
      key_ = key;
      modified_ = 0;
    }

    Node* left_child_;
    Node* right_child_;
    
    void Recalc(size_t length) {

      key_ = merge_(left_child_ ? left_child_->GetKey(length) : zero_element_,
                    right_child_ ? right_child_->GetKey(length) : zero_element_);

    }

    Element GetKey(size_t length = 0) const
    {
      if (modified_)
        return modify_(key_, modification_, length);
      else
        return key_;
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
  }; // Node

  void CreateTree(size_t size, const Element& zero_element) {
    size_ = size;
    zero_element_ = zero_element;
    BuildNodes(root_, Range(0, size_ - 1));
  }

  void RemoveTree(Node* root) {
    if (root) {
      RemoveTree(root->left_child_);
      RemoveTree(root->right_child_);
      root = NULL;
    }
  }

  void BuildNodes(Node* &root, Range current) {
    root = new Node(zero_element_);
    if (current.length() > 1) {
      BuildNodes(root->left_child_, current.LeftHalf());
      BuildNodes(root->right_child_, current.RightHalf());
    }
  }

  Element GetValue(Node* root, Range current, Range target, int type) {
    if (!root)
      return zero_element_;

    root->PushUpdate();
    root->Recalc(current.length());
    
    if (!target.IntersectWith(current))
      return zero_element_;
    
    target.CutWith(current);
    
    if (current == target) {
      //current.Print();
      //DB(root->GetKey(current.length()));
      return root->GetKey(current.length());
    }


    Element value = merge_(GetValue(root->left_child_, current.LeftHalf(), target), 
                          GetValue(root->right_child_, current.RightHalf(), target));

    //current.Print();
    //DB(value);
    return value;

    return merge_(GetValue(root->left_child_, current.LeftHalf(), target), 
                  GetValue(root->right_child_, current.RightHalf(), target));

  }

  void SetValue(Node* root, Range current, Range target, const Modification& modification) {
    if (!root)
      return;


    root->PushUpdate();

    if (!target.IntersectWith(current))
      return;

    target.CutWith(current);

    //current.Print();
    //target.Print();
    //DB("--");

    if (current == target) {
      root->AddModification(modification);
      //DB(root->GetKey(0));
      return;
    }
    
    SetValue(root->left_child_, current.LeftHalf(), target, modification);
    SetValue(root->right_child_, current.RightHalf(), target, modification);

    root->Recalc(current.length());
    //current.Print();
    //DB(root->GetKey(0));
  }

  Node* root_;
  size_t size_;
  static Element zero_element_;
  static MergeFunc merge_;
  static ModifyFunc modify_;
  static PushUpdateFunc update_;

}; // IntervalTree

template<typename Element, typename Modification,
         typename MergeFunc, typename ModifyFunc, typename PushUpdateFunc>
Element IntervalTree<Element, Modification, 
                    MergeFunc, ModifyFunc, PushUpdateFunc>::zero_element_;

#endif // INTERVAL_TREE_HPP_
