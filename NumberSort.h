#ifndef NUMBERSORT_H
#define	NUMBERSORT_H

class NumberSort{
private:
	int partCnt;
	int partSize;
public:
	NumberSort(int partCount, int bitsPerPart){
		partCnt = partCount;
		partSize = bitsPerPart;
	}

	template <typename T>
	unsigned getKey(T number, size_t pos) const {
		return (number >> ((partCnt - 1 - pos)*(partSize)))&((1LL << partSize) - 1);
	}

	unsigned getMaxKey(size_t) const {
		return (1LL << partSize) - 1;
	}

	template <typename Iterator>
	size_t getKeyCount(Iterator, Iterator) const{
		return partCnt;
	}

};
#endif	/* NUMBERSORT_H */

