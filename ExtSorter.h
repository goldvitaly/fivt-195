/* 
 * File:   ExtSorter.h
 * Author: riad
 *
 * Created on October 3, 2011, 11:18 AM
 */

#ifndef EXTSORTER_H
#define	EXTSORTER_H
#include <vector>
template <typename T>
class IO {
public:
	virtual ~IO() {
	};
	virtual void read(T& v) = 0;
	virtual void write(T& v) = 0;
	virtual bool ok()=0;
	virtual void toStart()=0;
	virtual void clear()=0;
};

template <typename T>
class ExtSorter {
	virtual IO<T>* generate() = 0;

	std::vector<IO<T>*> pts;
public:

	virtual ~ExtSorter() {
		for (auto it = pts.begin(); it != pts.end(); ++it)
			delete *it;
	}

	IO<T>* getNew() {
		pts.push_back(this->generate());
		return pts.back();
	}
};

#endif	/* EXTSORTER_H */
