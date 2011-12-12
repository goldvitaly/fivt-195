
#ifndef MINIMIZETREEASSIGNMODIFICATION_H
#define MINIMIZETREEASSIGNMODIFICATION_H
#include "IntervalTree.h"
template <typename T>
class MinimizeTreeAssignModification{
	struct Min{
		std::less<T> cmp;
		const T& operator() (const T& a, const T& b) const {
			return cmp(a,b)?a:b;
		}
	};

	struct SecondArgument{
		const T& operator() (const T& old, const T& newMod, size_t=0) const {
			return newMod;
		}
	};
	
	IntervalTree<T,T,Min,SecondArgument,SecondArgument> tree;
	
	public:
	explicit MinimizeTreeAssignModification(size_t count,T zero = T()):tree(count,zero){}

	template<typename Iterator>
	MinimizeTreeAssignModification(Iterator begin, Iterator end):tree(begin,end){}

	int getMin(size_t l, size_t r){
		return tree.get(l, r);
	}

	void assign(size_t l, size_t r, const T& value){
		tree.set(l,r,value);
	}

};

#endif	/* MINIMIZETREEASSIGNMODIFICATION_H */

