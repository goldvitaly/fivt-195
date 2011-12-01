/* 
 * File:   FileSource.h
 * Author: riad
 *
 * Created on November 27, 2011, 10:53 PM
 */

#ifndef FILESTORAGE_H
#define	FILESTORAGE_H
#include <fstream>
#include <memory>

class FileStorage{
public:
	std::string name;
	std::shared_ptr<std::fstream> stream;
public:
	explicit FileStorage(std::string name): name(name),stream(new std::fstream()){
		stream->open(name, std::ios_base::in | std::ios_base::out | std::ios_base::app);
	};

	void toStart(){
		stream->seekg(0);
		stream->seekp(0);
	}

	bool ok() const {
		return *stream;
	}

	void clear() {
		stream->close();
		stream->open(name, std::ios::in | std::ios::out | std::ios::trunc);
	}
};
#endif	/* FILESTORAGE_H */

