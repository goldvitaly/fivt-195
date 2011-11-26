/* 
 * File:   StringIterator.h
 * Author: riad
 *
 * Created on September 26, 2011, 11:31 AM
 */

#ifndef STRINGITERATOR_H
#define	STRINGITERATOR_H
#include <string>
namespace DigitSort {

	class StringSort{
	public:

		SortInt getMax(int) {
			return 255;
		}

		template <typename Iterator>
		SortInt getKey(Iterator iter, int pos) {
			if (pos < (iter->size()))
				return (SortInt) (unsigned char) ((*iter)[pos]);
			return 0;
		}

		template <typename Iterator>
		size_t getKeyCount(Iterator begin, Iterator end) {
			size_t maxLen = 0;
			for (Iterator it = begin; it != end; ++it) {
				maxLen = std::max(maxLen, it->size());
			}
			return maxLen;
		}
	};
};
#endif	/* STRINGITERATOR_H */

