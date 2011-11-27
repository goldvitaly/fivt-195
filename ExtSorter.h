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
	typedef IO<T>* FilePointer;
	virtual FilePointer generateFile() = 0;

	std::vector<FilePointer> pts;
public:

	virtual ~ExtSorter() {
		for (auto it = pts.begin(); it != pts.end(); ++it)
			delete *it;
	}

	FilePointer getNextFile() {
		pts.push_back(this->generateFile());
		return pts.back();
	}
};

#endif	/* EXTSORTER_H */
