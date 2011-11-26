/* 
 * File:   ByteFileSorter.h
 * Author: riad
 *
 * Created on October 3, 2011, 5:13 PM
 */

#ifndef BYTEFILESORTER_H
#define	BYTEFILESORTER_H
#include <iostream>
#include "ExtSorter.h"
#include <fstream>
#include <cstdio>

namespace ExternalSort {

	class ByteFileIO : public IO<unsigned> {
	public:
		enum class type{
			TEMPORARY,
			STABLE
		};
	private:
		std::fstream stream;
		std::string name;
		type temporary;
		
	public:

		ByteFileIO(std::string s, type temp) : name(s), temporary(temp) {
			if (temp==type::TEMPORARY)
				stream.open(s, std::ios_base::in | std::ios_base::out | std::ios_base::app);
			else
				stream.open(s, std::ios_base::in | std::ios_base::out);
		};

		~ByteFileIO() {
			stream.close();
			if (temporary==type::TEMPORARY)
				std::remove(name.c_str());
		}

		void read(unsigned& v) {
			v = (stream.get() << 24)+(stream.get() << 16)+(stream.get() << 8) + stream.get();
		}

		void write(unsigned& v) {
			stream.put((v >> 24)&0xFF);
			stream.put((v >> 16)&0xFF);
			stream.put((v >> 8)&0xFF);
			stream.put(v & 0xFF);
		}

		void toStart() {
			stream.seekg(0);
		}

		bool ok() {
			return !stream.eof();
		}

		void clear() {
			stream.close();
			stream.open(name, std::ios::in | std::ios::out | std::ios::trunc);
		}
	};

	class ByteFileSorter: public ExtSorter<unsigned> {
		std::string s;

		void next() {
			for (int i = 0; i < s.length(); ++i) {
				if (s[i] != 'z') {
					++s[i];
					return;
				}
				s[i] = 'a';
			}
			s += 'a';
		}

		IO<unsigned>* generate() {
			next();
			return new ByteFileIO(s, ByteFileIO::type::TEMPORARY);
		}

	};

};
#endif	/* BYTEFILESORTER_H */

