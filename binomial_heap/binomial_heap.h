#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H

#include "binomial_tree.h"
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <stdexcept>


template <class T, class Comp = std::less<T> >
class binomial_heap
{
  public:
		Comp comparator;
		void debug_write() const
		{
			for (auto i : trees)
			{
				std::cerr << "Tree:" << std::endl;
				i.debug_write();
			}
		}
		explicit binomial_heap(Comp c = Comp()): comparator(c)
		{
			size_ = 0;
		}
		int size() const 
		{
			return size_;
		};
		void insert(const T& value) 
		{
			size_ += 1;
			trees.push_back(tree(value, comparator));
			compress();
		}
		T min() const 
		{
			if (size_ == 0) throw std::logic_error("It's absolutely impossible to find minimal element in empty heap. But, if you want, I can try one more time");
			return trees[find_min_root_position()].min();
		};
		T extract_min()
		{
			if (size_ == 0) throw std::logic_error("It's absolutely impossible to remove element from empty heap. But, if you want, I can try one more time");
			size_ -= 1;
			auto min_root_iterator = trees.begin() + find_min_root_position();
			for (auto i : min_root_iterator->root->descendants)
				trees.push_back(tree(i, comparator));
			trees.erase(min_root_iterator);
			compress();
			return min_root_iterator->min();
		};

		static binomial_heap<T,Comp> merge(const binomial_heap<T,Comp>& a, const binomial_heap<T,Comp>& b)
		{
			if (a.size() == 0) return b;
			if (b.size() == 0) return a;
			binomial_heap<T,Comp> res;
			res.size_ = a.size_ + b.size_;
			res.trees = a.trees;
			res.trees.insert(res.trees.begin(), b.trees.begin(), b.trees.end());
			res.compress();
			return res;
		}
  private:
	typedef binomial_tree<T,Comp> tree;
	std::vector < tree > trees;
	int find_min_root_position() const
	{
		if (trees.size() == 0) throw std::logic_error("Heap is empty: finding is impossible");
		typename std::vector<tree>::const_iterator min_root_iterator = trees.begin();
		T min_value = trees.begin()->root->key;
		for (auto i = trees.begin(); i != trees.end(); i ++)
			if (comparator(i->root->key,min_value))
			{
				min_value = i->root->key;
				min_root_iterator = i;
			}
		return min_root_iterator - trees.begin();
	}
	void compress()
	{
		int max_order = 0;
		for (auto i = trees.begin(); i != trees.end(); ++ i)
			max_order = std::max(max_order, i -> order);
		max_order += 2;
		std::vector <tree> tree_of_order(max_order, tree(comparator));
		std::vector <bool> used(max_order, 0);
		DEBUG_CODE(int oldsize_=accumulate(trees.begin(), trees.end(), 0, [](int a, const tree& b){ return a + b.size(); }));
		for (auto added_tree = trees.begin(); added_tree != trees.end(); added_tree ++)
		{
			int pushed_tree = added_tree -> order;
			tree t = *added_tree;
			while (used[pushed_tree])
			{
				t = tree::merge(tree_of_order[pushed_tree], t);
				used[pushed_tree] = 0;
				pushed_tree ++;
			}
			tree_of_order[pushed_tree] = t;
			used[pushed_tree] = 1;
		}
		trees.clear();
		for (int i = 0; i < max_order; i ++)
			if (used[i])
				trees.push_back(tree_of_order[i]);
		DEBUG_CODE(int newsize_=accumulate(trees.begin(), trees.end(), 0, [](int a, tree b){ return a + b.size(); }));
		assert(newsize_ == oldsize_, "Summary size changed. I think that loss of some elements isn't a good idea. Or you created some new ones?");
		DEBUG_CODE(
				for (auto i = trees.begin(); i != trees.end(); i ++)
					for (auto j = trees.begin(); j != i; j ++)
						assert(i->size() != j->size(), "There are some equal trees after compress. Should you run it one more time?");
		);
	};
	int size_;
		
};


#endif
