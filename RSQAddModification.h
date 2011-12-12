#ifndef RSQADDMODIFICATION_H
#define RSQADDMODIFICATION_H

#include "IntervalTree.h"

template<typename T>
class RSQAddModification {

	struct AddSegment {

		T operator() (const T& val, const T& mod, size_t len) const {
			return val + mod * len;
		}
	};
	IntervalTree<T, T, std::plus<T>, AddSegment, std::plus<T> > tree;
public:

	explicit RSQAddModification(size_t n, T zero = T()) : tree(n, zero) {
	}

	template<typename Iterator>
	RSQAddModification(Iterator begin, Iterator end) : tree(begin, end) {
	}

	T getSum(size_t l, size_t r) {
		return tree.get(l, r);
	}

	void increase(size_t l, size_t r, const T& value) {
		tree.set(l, r, value);
	}
};

#endif