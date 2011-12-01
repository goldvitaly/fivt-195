#ifndef GENERATOR_H
#define	GENERATOR_H
#include <utility>
#include <string>
typedef unsigned long long ULL;
ULL genLong(){
	return ULL(rand())<<32+rand();
}

BigPair genPair(){
	return std::make_pair(genLong(),genLong());
}

std::string genString(){
	std::string s;
	for(int i=0;i<5;++i){
		s+=rand()%26+'a';
	}
	return s;
}

#endif	/* GENERATOR_H */

