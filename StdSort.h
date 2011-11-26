/* 
 * File:   StdSort.h
 * Author: riad
 *
 * Created on October 10, 2011, 10:52 AM
 */

#ifndef STDSORT_H
#define	STDSORT_H
namespace ExternalSort {

	template<typename T>
	class StdSort {
	public:

		std::greater<T> greater() const {
			return std::greater<T > ();
		}

		template<typename Iterator>
		void sort(Iterator a, Iterator b) {
			std::sort(a, b);
		}
	};
};
#endif	/* STDSORT_H */

