/* 
 * File:   FileSource.h
 * Author: riad
 *
 * Created on November 27, 2011, 10:53 PM
 */

#ifndef FILESOURCE_H
#define	FILESOURCE_H
#include <fstream>
#include <memory>

#include <iostream>
#include <cstdlib>

class FileSource{
public:
	std::string name;
	std::shared_ptr<std::fstream> stream;
public:
	FileSource(std::string name): name(name),stream(new std::fstream()){
		stream->open(name, std::ios_base::in | std::ios_base::out | std::ios_base::app);
	};

	void toStart() {
		stream->seekg(0);
	}

	bool ok() {
		return *stream;
	}

	void clear() {
		stream->close();
		stream->open(name, std::ios::in | std::ios::out | std::ios::trunc);
	}
};
#endif	/* FILESOURCE_H */

