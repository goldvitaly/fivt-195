#ifndef MINIMIZETREEADDMODIFICATION_H
#define MINIMIZETREEADDMODIFICATION_H

#include "IntervalTree.h"
template <typename T>
class MinimizeTreeAddModification{
	struct Min{
		std::less<T> cmp;
		const T& operator() (const T& a, const T& b) const {
			return cmp(a,b)?a:b;
		}
	};

	struct SumIgnore{
		T operator() (const T& a, const T& b, size_t) const {
			return a+b;
		}
	};

	IntervalTree<T,T,Min,SumIgnore,std::plus<T>> tree;

	public:
	explicit MinimizeTreeAddModification(size_t count,const T& zero = T()):tree(count, zero){}

	template<typename Iterator>
	MinimizeTreeAddModification(Iterator begin, Iterator end):tree(begin, end){}

	int getMin(size_t l, size_t r){
		return tree.get(l, r);
	}

	void increase(size_t l, size_t r, const T& value){
		tree.set(l,r,value);
	}

};

#endif /* MINIMIZETREEADDMODIFICATION_H */

