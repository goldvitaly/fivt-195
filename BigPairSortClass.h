#ifndef BIGPAIRSORTCLASS_H
#define	BIGPAIRSORTCLASS_H

/**
 * for pairs of uns long long, that maybe to max of long long
 * Sort by 16bits blocks
 */
class BigPairSort{
public:

	SortInt getMax(int) {
		return (1 << 16) - 1;
	}

	template <typename Iterator>
	SortInt getKey(Iterator iter, int pos) {
		if (pos < 4) {
			return (iter->first >> ((3 - pos)*16))&0xFFFF;
		}
		pos -= 4;
		return (iter->second >> ((3 - pos)*16))&0xFFFF;
	}

	template <typename Iterator>
	size_t getKeyCount(Iterator begin, Iterator end) {
		return 8;
	}
};
#endif	/* BIGPAIRSORTCLASS_H */