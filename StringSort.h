#ifndef STRINGSORT_H
#define	STRINGSORT_H
#include <string>

class StringSort{
public:

	unsigned getMaxKey(size_t) const {
		return 255;
	}

	unsigned getKey(const std::string& str, size_t pos) const{
		if (pos < (str.size()))
			return (int) (unsigned char) (str[pos]);
		return 0;
	}

	template <typename Iterator>
	size_t getKeyCount(Iterator begin, Iterator end) const{
		size_t maxLen = 0;
		for (Iterator it = begin; it != end; ++it) {
			maxLen = std::max(maxLen, it->size());
		}
		return maxLen;
	}
};

#endif	/* STRINGSORT_H */