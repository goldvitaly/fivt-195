#ifndef MONTHSORT_H
#define	MONTHSORT_H

class MonthSort  {
public:

	unsigned getKey(std::pair<int,int> element, int pos) {
		return pos ? element.second : element.first;
	}

	unsigned getMaxKey(int position) {
		if (position)
			return 12; // Месяцы
		else
			return 300; //Года
	}

	template <typename Iterator>
	size_t getKeyCount(Iterator begin, Iterator end) {
		return 2;
	}
};


#endif	/* MONTHSORT_H */

