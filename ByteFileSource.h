/* 
 * File:   ByteFileSource.h
 * Author: riad
 *
 * Created on November 27, 2011, 11:20 PM
 */

#ifndef BYTEFILESOURCE_H
#define	BYTEFILESOURCE_H
#include "FileSource.h"

class ByteFileSource : public FileSource{
	public:

		ByteFileSource(const std::string& name) :  FileSource(name){
			
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

#endif	/* BYTEFILESOURCE_H */

