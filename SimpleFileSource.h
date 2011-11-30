/* 
 * File:   SimpleFileSource.h
 * Author: riad
 *
 * Created on November 27, 2011, 11:02 PM
 */

#ifndef SIMPLEFILESOURCE_H
#define	SIMPLEFILESOURCE_H

#include "FileSource.h"

template <typename T>
class SimpleFileSource : public FileSource{
	public:
		SimpleFileSource(const std::string& s) : FileSource(s){

		}
	void read(T& v) {
		(*stream)>>v;
	}

	void write(T& v) {
		(*stream)<<v<<' ';
	}
};

#endif	/* SIMPLEFILESOURCE_H */

