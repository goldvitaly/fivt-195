/* 
 * File:   SimpleFileSource.h
 * Author: riad
 *
 * Created on November 27, 2011, 11:02 PM
 */

#ifndef SIMPLEFILESTORAGE_H
#define	SIMPLEFILESTORAGE_H

#include "FileStorage.h"

template <typename T>
class SimpleFileStorage : public FileStorage{
	public:
		SimpleFileStorage(const std::string& s) : FileStorage(s){

		}
	void read(T& v) {
		(*stream)>>v;
	}

	void write(T& v) {
		(*stream)<<v<<' ';
	}
};

#endif	/* SIMPLEFILESTORAGE_H */

