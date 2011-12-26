#ifndef EXTR_H
#define EXTR_H

using namespace std;

// Extractors

template<size_t MaxLength>
int stringExtractor (const string& s, size_t BlockNumber)
{
	if (BlockNumber >= MaxLength)
		return -1;
	else if (BlockNumber < MaxLength - s.length())
		return 0;
	else
		return s[MaxLength - BlockNumber - 1];
}
template<typename T>
inline int integerExtractor (const T& a, size_t BlockNumber)
{
	if (BlockNumber >= sizeof(T))
		return -1;
	return (a >> (BlockNumber * sizeof(T))) & 0xFF;
}
inline int intExtractor (const int& a, size_t BlockNumber)
{
	return integerExtractor<int> (a, BlockNumber);
}
inline int i64Extractor (const long long& a, size_t BlockNumber)
{
	return integerExtractor<long long> (a, BlockNumber);
}
inline int pairExtractor (const pair<int, int>& a, size_t BlockNumber)
{
	if (BlockNumber > 7)
		return -1;
	else if (BlockNumber < 4)
		return intExtractor(a.second, BlockNumber);
	else
		return intExtractor(a.first, BlockNumber - 4);
}

// Random enerators

long long i64Rand()
{
	return ((long long)(rand()) << 32) + rand();
}
template<int length>
string strRand()
{
	string t(length, '0');
	for (int i = 0; i < length; i++)
		t[i] = 'a' + rand()%('z' - 'a');
	return t;
}
pair<int, int> pairRand()
{
	return make_pair(rand(), rand());
}

#endif
