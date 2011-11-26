/* 
 * File:   ExtSorter.h
 * Author: riad
 *
 * Created on October 3, 2011, 10:48 AM
 */

#ifndef FILEEXTSORTER_H
#define	FILEEXTSORTER_H
#include <iostream>
#include "ExtSorter.h"
#include <fstream>
#include <cstdio>

namespace ExternalSort {

	template <typename T>
	class FileIO : public IO<T> {
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

		

		FileIO(std::string s, type temp) : name(s), temporary(temp) {
			if (temp==type::TEMPORARY)
				stream.open(s, std::ios_base::in | std::ios_base::out | std::ios_base::app);
			else
				stream.open(s, std::ios_base::in | std::ios_base::out);
		};

		~FileIO() {
			stream.close();
			if (temporary==type::TEMPORARY)
				std::remove(name.c_str());
		}

		void read(T& v) {
			stream >> v;
		}

		void write(T& v) {
			stream << v << ' ';
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

	template <typename T>
	class FileExtSorter : public ExtSorter<T> {
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

		IO<T>* generate() {
			next();
			return new FileIO<T > (s, FileIO<T>::type::TEMPORARY);
		}

	};
};
#endif	/* FILEEXTSORTER_H */

