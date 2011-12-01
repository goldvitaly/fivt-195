#ifndef NUMBERSORT_H
#define	NUMBERSORT_H

class NumberSort{
private:
	int partCnt;
	int partSize;
public:
	NumberSort(int partCount, int bitsPerPart) {
		partCnt = partCount;
		partSize = bitsPerPart;
	}

	template <typename T>
	SortInt getKey(T number, size_t pos) {
		return (number >> ((partCnt - 1 - pos)*(partSize)))&((1LL << partSize) - 1);
	}

	SortInt getMaxKey(size_t) {
		return (1LL << partSize) - 1;
	}

	template <typename Iterator>
	size_t getKeyCount(Iterator, Iterator) {
		return partCnt;
	}

};
#endif	/* NUMBERSORT_H */

