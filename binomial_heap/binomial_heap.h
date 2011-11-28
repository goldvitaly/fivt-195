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
	typedef Comp comparator;
	typedef binomial_tree<T,Comp> tree;
	std::vector< binomial_tree<T,Comp> > trees;
	void compress()
	{
		int max_order = 0;
		for (auto i = trees.begin(); i != trees.end(); ++ i)
			max_order = std::max(max_order, i -> order);
		max_order += 3;
		std::vector <binomial_tree<T,Comp>> tree_of_order(max_order);
		std::vector <bool> used(max_order, 0);
		DEBUG_CODE(int oldsize_=accumulate(trees.begin(), trees.end(), 0, [](int a, tree b){ return a + b.size(); }));
		for (auto i = trees.begin(); i != trees.end(); i ++)
		{
			int j = i -> order;
			binomial_tree<T,Comp> t = *i;
			while (used[j])
			{
				t = binomial_tree<T,Comp>::merge(tree_of_order[j], t);
				used[j] = 0;
				j ++;
			}
			tree_of_order[j] = t;
			used[j] = 1;
		}
		trees.resize(0);
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
	public:
		void debug_write() const
		{
			for (auto i : trees)
			{
				std::cerr << "Tree:" << std::endl;
				i.debug_write();
			}
		}
		binomial_heap(): trees()
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
			trees.push_back(binomial_tree<T,Comp>(value));
			compress();
		}
		T 	 min() const 
		{
			if (size_ == 0) throw std::logic_error("It's absolutely impossible to find minimal element in empty heap. But, if you want, I can try one more time");
			T min_value = trees.begin()->root->key;
			for (auto i = trees.begin(); i != trees.end(); i ++)
				if (comparator()(i->root->key,min_value))
					min_value = i->root->key;
			return min_value;
		};
		T extract_min()
		{
			if (size_ == 0) throw std::logic_error("It's absolutely impossible to remove element from empty heap. But, if you want, I can try one more time");
			size_ -= 1;
			T min_value = trees.begin()->root->key;
			auto it = trees.begin();
			for (auto i = trees.begin(); i != trees.end(); i ++)
				if (comparator()(i->root->key,min_value))
				{
					min_value = i->root->key;
					it = i;
				}
			int pos = it - trees.begin();
			for (auto i : it->root->descendants)
				trees.push_back(binomial_tree<T,Comp>(i));
			trees.erase(pos + trees.begin());
			compress();
			return min_value;
		};

		static binomial_heap<T,Comp> merge(binomial_heap<T,Comp> a, binomial_heap<T,Comp> b)
		{
			if (a.size() == 0) return b;
			if (b.size() == 0) return a;
			binomial_heap<T,Comp> res;
			res.size_ = a.size_ + b.size_;
			res.trees = a.trees;
			for (auto i = b.trees.begin(); i != b.trees.end(); i ++)
				res.trees.push_back(*i);
			res.compress();
			return res;
		}

};


#endif
