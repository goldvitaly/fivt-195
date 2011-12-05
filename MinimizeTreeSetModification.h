
#ifndef MINIMIZETREESETMODIFICATION_H
#define MINIMIZETREESETMODIFICATION_H
#include "IntervalTree.h"
template <typename T>
class MinimizeTreeSetModification{
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
	MinimizeTreeSetModification(size_t n,T Zero = T()):tree(n,Zero){}

	int getMin(size_t l, size_t r){
		return tree.get(l, r);
	}

	void setValue(size_t l, size_t r, const T& value){
		tree.set(l,r,value);
	}

};

#endif	/* MINIMIZETREESETMODIFICATION_H */

