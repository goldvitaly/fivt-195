#ifndef STRINGSORT_H
#define	STRINGSORT_H
#include <string>

class StringSort{
public:

	SortInt getMaxKey(size_t) {
		return 255;
	}

	SortInt getKey(const std::string& str, size_t pos) {
		if (pos < (str.size()))
			return (SortInt) (unsigned char) (str[pos]);
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

#endif	/* STRINGSORT_H */

