#ifndef RSQASSIGNMODIFICATION_H
#define RSQASSIGNMODIFICATION_H

#include "IntervalTree.h"

template<typename T>
class RSQAssignModification {

	struct SecondArgument{
		const T& operator() (const T& old, const T& newMod) const {
			return newMod;
		}
	};
	struct AssignedInterval{
		T operator()(const T& old, const T& mod, size_t len) const {
			return mod*len;
		}
	};
	IntervalTree<T, T, std::plus<T>, AssignedInterval , SecondArgument > tree;
public:

	explicit RSQAssignModification(size_t count, const T& zero = T()) : tree(count, zero) {
	}

	template<typename Iterator>
	RSQAssignModification(Iterator begin, Iterator end) : tree(begin, end) {
	}

	T getSum(size_t l, size_t r) {
		return tree.get(l, r);
	}

	void assign(size_t l, size_t r, const T& value) {
		tree.set(l, r, value);
	}
};

#endif /* RSQASSIGNMODIFICATION_H */

