#ifndef MINIPAIRSORT_H
#define	MINIPAIRSORT_H

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


#endif	/* MINIPAIRSORT_H */

