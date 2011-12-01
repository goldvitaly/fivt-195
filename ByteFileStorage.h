/* 
 * File:   ByteFileSource.h
 * Author: riad
 *
 * Created on November 27, 2011, 11:20 PM
 */

#ifndef BYTEFILESTORAGE_H
#define	BYTEFILESTORAGE_H
#include "FileStorage.h"

class ByteFileStorage : public FileStorage{
	public:

		explicit ByteFileStorage(const std::string& name) :  FileStorage(name){
			
		}
	void read(unsigned& v) {
		v = (stream->get() << 24)+(stream->get() << 16)+(stream->get() << 8) + stream->get();
	}

	void write(const unsigned& v) {
		stream->put((v >> 24)&0xFF);
		stream->put((v >> 16)&0xFF);
		stream->put((v >> 8)&0xFF);
		stream->put(v & 0xFF);
	}
};

#endif	/* BYTEFILESTORAGE_H */

