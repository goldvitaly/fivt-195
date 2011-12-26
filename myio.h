#ifndef MYIO_H
#define MYIO_H

/** My input and output functions
 * 	ostream is used for input and output respectively
 * 	@note Big Endian is used
 */

#include <cstring>
#include <cassert>
#include <iostream>

template<typename T>
T readInteger(std :: istream& in)
{
	T res = 0;
	for (size_t i = 0; i < sizeof(T); i++)
		res = (res << 8) | (in.get());
	return res;
}
int readInt(std :: istream& in)
{
	return readInteger<int>(in);
}
long long readI64(std :: istream& in)
{
	return readInteger<long long>(in);
}
long long readI64(char* s)
{
	long long result = 0;
	int len = strlen(s);
	for (int i = 0; i < len; i++)
	{
		assert(isdigit(s[i]));
		result = result * 10 + s[i] - '0';
	}
	return result;
}

template<typename T>
void writeInteger (std :: ostream& out, T x)
{
	out.write((char*)x, sizeof(T));
}
void writeInt(std :: ostream& out, int x)
{
	writeInteger<int>(out, x);
}
void writeI64(std :: ostream& out, long long x)
{
	writeInteger<long long>(out, x);
}

std :: streamoff fileSize(std :: iostream& st)
{
	std :: streampos tmp = st.tellg();
	st.seekg(0, st.end);
	std :: streampos end = st.tellg();
	st.seekg(0, st.beg);
	std :: streampos beg = st.tellg();
	
	st.seekg(tmp);
	return end - beg;
}

int peekInt(std :: istream &st)
{
	int t = readInt(st);
	st.seekg(-4, st.cur);
	return t;
}

void printFile(std :: istream& st, std :: streampos b, std :: streampos e, std :: ostream& out)
{
	std :: streampos tmp = st.tellg();
	if (e == -1)
	{
		st.seekg(0, st.end);
		e = st.tellg() / 4;
	}

	assert(b <= e);
	
	
	st.seekg(b*4, st.beg);
	for (; b != e; b += 1)
		out << readInt(st) << " ";
	out << std :: endl;
	
	st.seekg(tmp);
}
void printFile (std :: istream& st)
{
	printFile(st, 0, -1, std :: cout);
}
void printFile(std :: istream& st, std :: streampos b, std :: streampos e)
{
	printFile(st, b, e, std :: cout);
}
void printFile(std :: istream& st, std :: ostream& out)
{
	printFile(st, 0, -1, out);
}
void copy (const char* src, const char* dst)
{
	std :: fstream in (src, std :: ios :: in | std :: ios :: binary);
	std :: fstream out (dst, std :: ios :: out | std :: ios :: binary);
	
	int sz = fileSize(in);
	for (int i = 0; i < sz; i++)
		out.put(in.get());
	
	in.close();
	out.close();
	
}
#endif
