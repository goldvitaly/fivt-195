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
	typedef Comp comparator;
	struct node
	{
		int order;
		std::vector < std::shared_ptr<node> > descendants;
		T key;
		node(const T& k, int ord = 0)
		{
			key = k;
			order = ord;
		}
	};
	typedef std::shared_ptr<node> pnode;

	pnode root;
	int order;
	static void check_heap_property(pnode v) 
	{
		if (!v) return;
		for (auto i = v->descendants.begin(); i != v->descendants.end(); i ++)
		{
			assert(!comparator()((*i) -> key, v->key), "Heap property is broken. Keys are " << v->key << " " << (*i) -> key);
			check_heap_property(*i);
		}
	};
	void check_heap_property() const 
	{
		check_heap_property(root);
	};	
	
	/* ONLY FOR DEBUGGING PURPOSES! */
	void debug_write_ptr(pnode v, int shift = 0)
	{
		for (int i = 0; i < shift; i ++) std::cerr << "   "; std::cerr << v->key << std::endl;
		for (auto i : v->descendants)
			debug_write_ptr(i, shift + 1);
	}
  public:
	/* ONLY FOR DEBUGGING PURPOSES! */
	void debug_write() const
	{
		debug_write_ptr(root);
	};
	explicit binomial_tree(pnode v = pnode(0))
	{
		root = v;
		if (v)
			order = v -> order;
		else
			order = 0;
	};
	explicit binomial_tree(const T& value)
	{
		root = std::shared_ptr<node> (new node(value));
		order = 0;
	};
	int size() const 
	{
		if (!root) return 0;
		return (1 << root->order);
	};
	static binomial_tree<T,Comp> merge(const binomial_tree<T,Comp>& a, const binomial_tree<T,Comp>& b) 
	{
		if (!a.root) return b;
		if (!b.root) return a;
		assert(a.order == b.order, "Merging two trees with unequal size is absolutely impossible");
		binomial_tree<T,Comp> res;
		res.order = a.order + 1; 
		if (!comparator()(b.root->key,a.root->key))
		{
			pnode v(new node(a.root->key));
			v->descendants = a.root->descendants;
			v->descendants.push_back(b.root);
			v->order = a.order + 1;
			res = binomial_tree<T,Comp>(v);
		}
		else
			return merge(b, a);
		DEBUG_CODE(res.check_heap_property());		
		return res;
	}
};



#endif

