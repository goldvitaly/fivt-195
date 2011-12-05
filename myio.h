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

int readInt(istream& in)
{
	int res = 0;
	for (int i = 0; i < 4; i++)
		res = (res << 8) | (in.get());
	return res;
}
int readI64(istream& in)
{
	long long res = 0;
	for (int i = 0; i < 8; i++)
		res = (res << 8) | (in.get());
	return res;
}
long long readInt(char* s)
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


void writeInt(ostream& out, int x)
{
	for (int i = 8*3, j = 0xFF000000; i >= 0; i -= 8, j >>= 8)
	{
		char toWrite = (x & j) >> i;
		out.put(toWrite);
	}	
}
void writeI64(ostream& out, long long x)
{
	for (long long i = 8*7, j = 0xFF00000000000000ll; i >= 0; i -= 8, j >>= 8)
	{
		char toWrite = (x & j) >> i;
		out.put(toWrite);
	}	
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
void advp(ostream& st, int k)
{
	st.seekp(k, st.cur);
}
void advg(istream& st, int k)
{
	st.seekg(k, st.cur);
}

int peekInt(istream &st)
{
	int t = readInt(st);
	advg(st, -4);
	return t;
}

void printFile(istream& st, streampos b = 0, streampos e = -1, ostream& out = cout)
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
