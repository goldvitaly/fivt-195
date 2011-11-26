/* 
 * File:   NumberSort.h
 * Author: riad
 *
 * Created on September 26, 2011, 4:30 PM
 */

#ifndef NUMBERSORT_H
#define	NUMBERSORT_H

namespace DigitSort {

	class NumberSort{
	private:
		int cnt;
		int size; // bites
	public:

		/**
		 *
		 * @param C part counts
		 * @param S Size of every part(in bites)
		 */
		NumberSort(int C, int S) {
			cnt = C;
			size = S;
		}

		template <typename Iterator>
		SortInt getKey(Iterator iter, int pos) {
			return ((*iter) >> ((cnt - 1 - pos)*(size)))&((1LL << size) - 1);
		}

		SortInt getMax(int) {
			return (1LL << size) - 1;
		}

		template <typename Iterator>
		size_t getKeyCount(Iterator begin, Iterator end) {
			return cnt;
		}

	};
};
#endif	/* NUMBERSORT_H */

