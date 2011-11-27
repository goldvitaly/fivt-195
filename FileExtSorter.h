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


template <typename T>
class FileIO : public IO<T> {
public:
	enum class permanence{
		TEMPORARY,
		PERMANENT
	};
private:
	std::fstream stream;
	std::string name;
	permanence perm;
public:

	FileIO(std::string s, permanence perm) : name(s), perm(perm) {
		if (perm==permanence::TEMPORARY)
			stream.open(s, std::ios_base::in | std::ios_base::out | std::ios_base::app);
		else
			stream.open(s, std::ios_base::in | std::ios_base::out);
	};

	~FileIO() {
		stream.close();
		if (perm==permanence::TEMPORARY)
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

	void nextName() {
		for (int i = 0; i < s.length(); ++i) {
			if (s[i] != 'z') {
				++s[i];
				return;
			}
			s[i] = 'a';
		}
		s += 'a';
	}

	IO<T>* generateFile() {
		nextName();
		return new FileIO<T>(s, FileIO<T>::permanence::TEMPORARY);
	}

};
#endif	/* FILEEXTSORTER_H */

