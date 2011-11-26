/* 
 * File:   MiniPairSort.h
 * Author: riad
 *
 * Created on September 26, 2011, 11:34 AM
 */

#ifndef MINIPAIRSORT_H
#define	MINIPAIRSORT_H

namespace DigitSort {

	class YearSort  {
	public:

		template <typename Iterator>
		SortInt getKey(Iterator iter, int pos) {
			return pos ? iter->second : iter->first;
		}

		SortInt getMax(int pos) {
			if (pos)
				return 12; // Месяцы
			else
				return 300; //Года
		}

		template <typename Iterator>
		size_t getKeyCount(Iterator begin, Iterator end) {
			return 2;
		}
	};

};
#endif	/* MINIPAIRSORT_H */

