#ifndef BINOMIAL_TREE_H
#define BINOMIAL_TREE_H

#include "assert.h"
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>

template <class T, class Comp> class binomial_heap;

template <class T, class Comp = std::less<T> >
class binomial_tree
{
	friend class binomial_heap<T,Comp>;
	Comp comparator;
	struct node
	{
		int order;
		std::vector < std::shared_ptr<node> > descendants;
		T key;
		explicit node(const T& key, int order = 0): key(key), order(order)
		{
		}
	};
	typedef std::shared_ptr<node> pnode;

	pnode root;
	int order;

	T min() const
	{
		return root->key;
	}


	void check_heap_property(pnode v) const
	{
		if (!v) return;
		for (auto i = v->descendants.begin(); i != v->descendants.end(); i ++)
		{
			assert(!comparator((*i) -> key, v->key), "Heap property is broken. Keys are " << v->key << " " << (*i) -> key);
			check_heap_property(*i);
		}
	};
	/* ONLY FOR DEBUGGING PURPOSES! */
	void debug_write_ptr(pnode v, int shift = 0) const
	{
		for (int i = 0; i < shift; i ++) std::cerr << "   "; std::cerr << v->key << std::endl;
		for (auto i : v->descendants)
			debug_write_ptr(i, shift + 1);
	}
  public:
	void check_heap_property() const 
	{
		check_heap_property(root);
	};	
	/* ONLY FOR DEBUGGING PURPOSES! */
	void debug_write() const
	{
		debug_write_ptr(root);
	};
	explicit binomial_tree(Comp comparator): comparator(comparator)
	{
		root = pnode(0);
		order = 0;
	}

	explicit binomial_tree(pnode v, Comp comparator): comparator(comparator)
	{
		root = v;
		order = v ? v -> order : 0;
	};
	explicit binomial_tree(const T& value, Comp comparator): comparator(comparator)
	{
		root = std::shared_ptr<node> (new node(value));
		order = 0;
	};
	int size() const 
	{
		if (!root) return 0;
		return (1 << root->order);
	};
	static binomial_tree<T,Comp> merge(const binomial_tree<T,Comp>& first_tree, const binomial_tree<T,Comp>& second_tree) 
	{
		if (!first_tree.root) return second_tree;
		if (!second_tree.root) return first_tree;
		assert(first_tree.order == second_tree.order, "Merging two trees with unequal size is absolutely impossible");
		binomial_tree<T,Comp> res(first_tree.comparator);
		res.order = first_tree.order + 1; 
		// we are using first_tree tree comparator, they should be same
		if (!first_tree.comparator(second_tree.root->key,first_tree.root->key))
		{
			pnode v(new node(first_tree.root->key));
			v->descendants = first_tree.root->descendants;
			v->descendants.push_back(second_tree.root);
			v->order = first_tree.order + 1;
			res = binomial_tree<T,Comp>(v, first_tree.comparator);
		}
		else
			return merge(second_tree, first_tree);
		DEBUG_CODE(res.check_heap_property());		
		return res;
	}
};



#endif

