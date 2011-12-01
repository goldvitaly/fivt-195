#ifndef BIGPAIRSORTCLASS_H
#define	BIGPAIRSORTCLASS_H
#include <utility>
/**
 * for pairs of uns long long, that maybe to max of long long
 * Sort by 16bits blocks
 */
class BigPairSort{
public:
	typedef std::pair<unsigned long long, unsigned long long> BigPair;
	SortInt getMaxKey(size_t) {
		return (1 << 16) - 1;
	}

	SortInt getKey(BigPair element, size_t pos) {
		if (pos < 4) {
			return (element.first >> ((3 - pos)*16))&0xFFFF;
		}
		pos -= 4;
		return (element.second >> ((3 - pos)*16))&0xFFFF;
	}

	template <typename Iterator>
	size_t getKeyCount(Iterator begin, Iterator end) {
		return 8;
	}
};
#endif	/* BIGPAIRSORTCLASS_H */