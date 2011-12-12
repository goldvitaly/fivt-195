
#ifndef MINIMIZETREESETMODIFICATION_H
#define MINIMIZETREESETMODIFICATION_H
#include "IntervalTree.h"
template <typename T>
class MinimizeTreeAssignModification{
	struct Min{
		const T& operator() (const T& a, const T& b) const {
			return a<b?a:b;
		}
	};

	struct SecondArgument{
		const T& operator() (const T& old, const T& newMod,size_t=0) const {
			return newMod;
		}
	};
	
	IntervalTree<T,T,Min,SecondArgument,SecondArgument> tree;
	
	public:
	MinimizeTreeAssignModification(size_t n,T zero = T()):tree(n,zero){}

	template<typename Iterator>
	MinimizeTreeAssignModification(Iterator begin, Iterator end):tree(begin,end){}

	int getMin(size_t l, size_t r){
		return tree.get(l, r);
	}

	void setValue(size_t l, size_t r, const T& value){
		tree.set(l,r,value);
	}

};

#endif	/* MINIMIZETREESETMODIFICATION_H */

