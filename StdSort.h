/* 
 * File:   StdSort.h
 * Author: riad
 *
 * Created on October 10, 2011, 10:52 AM
 */

#ifndef STDSORT_H
#define	STDSORT_H
#include <algorithm>
template<typename T, typename cmp=std::less<T> >
class StdSort {
public:

	cmp less() const {
		return cmp();
	}

	template<typename Iterator>
	void sort(Iterator a, Iterator b) {
		std::sort(a, b, less());
	}
};
#endif	/* STDSORT_H */

