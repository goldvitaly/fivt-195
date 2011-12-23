#include "digit.h"
#include <iostream>
#include <algorithm>
#include <ctime>

using namespace std;

template<>
struct Extractor<int>
{
	public:
		static const unsigned digit_count = 31;
		static const unsigned digit_range = 2;
		int operator()(int number, unsigned digit)
		{
			return (number & (1 << digit)) >> digit;
		}
};

template<>
struct Extractor<string>
{
	static const unsigned digit_count = 7;
	static const unsigned digit_range = 256;
	int operator()(string s, unsigned digit)
	{
		return s[digit_count - 1 - digit];
	}
};

template<class T>
struct Extractor<pair<T, T> >
{
	static const unsigned digit_count = Extractor<T>::digit_count * 2;
	static const unsigned digit_range = Extractor<T>::digit_range;
	T operator()(pair<T, T> pair, unsigned digit)
	{
		if(digit < Extractor<T>::digit_count)
			return single_extr(pair.first, digit);
		else
			return single_extr(pair.second, digit-Extractor<T>::digit_count);
	}
	private:
		Extractor<T> single_extr;
};

template<class T>
T fromSeed(int seed);

template<>
int fromSeed<int>(int seed)
{
	return seed;
}

template<>
string fromSeed<string>(int seed)
{
	string s = "";
	for(unsigned i = 0;i < Extractor<string>::digit_count; i++)
		s+= 97 + (char)rand()%25;
	return s;
}

template<>
pair<int, int> fromSeed<pair<int, int> >(int seed)
{
	return make_pair(fromSeed<int>(seed), fromSeed<int>(seed + 1));
}

template<class T>
bool primitiveTest(int test_size)
{
	T* arr = new T[test_size];
	T* chk = new T[test_size];
	
	for(int i = 0; i < test_size; i++)
	{
		arr[i] = chk[i] = fromSeed<T>(rand());
	}
	digitSort<T, Extractor<T> >(arr,arr+test_size, Extractor<T>());
	std::sort(chk, chk + test_size);
	
	bool good = true;
	for(int i = 0; i < test_size; i++)
	{
		if(arr[i] != chk[i])
		{
			cerr << "array[" << i << "] is wrong" << endl;
			good = false;
			break;
		}
	}
	
	/*cerr << "Printing array beginnings..." << endl << "std: " << endl;
	for(int i = 0; i < min(10, test_size); i++)
		cerr << chk[i] << " ";
	cerr << endl << "custom:" << endl;
	for(int i = 0; i < min(10, test_size); i++)
		cerr << arr[i] << " ";
	cerr << endl;*/
	
	delete[] arr;
	delete[] chk;
	return good;
}

void show_time()
{
	static long long t;
	cerr << "Time elapsed: " << (clock() - t) * 1000 / CLOCKS_PER_SEC << endl;
	t = clock();
}

int main()
{
	show_time();
	int test_size = 1000000;
	
	cerr << "Test array size: " << test_size << endl;
	
	if(!primitiveTest<string>(test_size))
	{
		cerr << "Failed primitive string test." << endl;
		return -1;
	}
	cerr << "Primitive string test success." << endl;
	show_time();
	
	if(!primitiveTest<int>(test_size))
	{
		cerr << "Failed primitive int test." << endl;
		return -1;
	}
	cerr << "Primitive int test success." << endl;
	show_time();
	
	if(!primitiveTest<pair<int, int> >(test_size))
	{
		cerr << "Failed primitive int pair test." << endl;
		return -1;
	}
	cerr << "Primitive int pair test success." << endl;
	show_time();
	
	return 0;
}
