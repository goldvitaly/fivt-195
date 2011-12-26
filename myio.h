#ifndef MYIO_H
#define MYIO_H

/** My input and output functions
 * 	ostream is used for input and output respectively
 * 	@note Big Endian is used
 */

#include <cstring>
#include <cassert>
#include <iostream>
using namespace std;

template<typename T>
T readInteger(istream& in)
{
	T res = 0;
	for (size_t i = 0; i < sizeof(T); i++)
		res = (res << 8) | (in.get());
	return res;
}
int readInt(istream& in)
{
	return readInteger<int>(in);
}
long long readI64(istream& in)
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
void writeInteger (ostream& out, T x)
{
	out.write((char*)x, sizeof(T));
}
void writeInt(ostream& out, int x)
{
	writeInteger<int>(out, x);
}
void writeI64(ostream& out, long long x)
{
	writeInteger<long long>(out, x);
}

streamoff fileSize(iostream& st)
{
	streampos tmp = st.tellg();
	st.seekg(0, st.end);
	streampos end = st.tellg();
	st.seekg(0, st.beg);
	streampos beg = st.tellg();
	
	st.seekg(tmp);
	return end - beg;
}

int peekInt(istream &st)
{
	int t = readInt(st);
	st.seekg(-4, st.cur);
	return t;
}

void printFile(istream& st, streampos b, streampos e, ostream& out)
{
	streampos tmp = st.tellg();
	if (e == -1)
	{
		st.seekg(0, st.end);
		e = st.tellg() / 4;
	}

	assert(b <= e);
	
	
	st.seekg(b*4, st.beg);
	for (; b != e; b += 1)
		out << readInt(st) << " ";
	out << endl;
	
	st.seekg(tmp);
}
void printFile (istream& st)
{
	printFile(st, 0, -1, cout);
}
void printFile(istream& st, streampos b, streampos e)
{
	printFile(st, b, e, cout);
}
void printFile(istream& st, ostream& out)
{
	printFile(st, 0, -1, out);
}
void copy (const char* src, const char* dst)
{
	fstream in (src, ios :: in | ios :: binary);
	fstream out (dst, ios :: out | ios :: binary);
	
	int sz = fileSize(in);
	for (int i = 0; i < sz; i++)
		out.put(in.get());
	
	in.close();
	out.close();
	
}
#endif
