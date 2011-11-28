#include <vector>
#include <algorithm>
#include <string.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <cassert>

class int_extractor
{
	static const int bits = 32;
	const int block_size;
	const int max_blocks;
public:
	int_extractor(int block_size_ = 8): block_size(block_size_), max_blocks(bits / block_size_ + bool(bits % block_size_)) { };
	inline int get_block_size() const  { return block_size; };
	inline int blocks(const int& a) const { return max_blocks; }
	inline int operator ()(const int& a, const int& block_number, const int& blocks) const { return (a >> (block_size * block_number)) & ((1 << block_size) - 1); };
};


class long_long_extractor
{
	static const int bits = 64;
	const int max_blocks;
	const int block_size;
public:
	long_long_extractor(int block_size_ = 8): block_size(block_size_), max_blocks(bits / block_size_ + bool(bits % block_size_)) { };
	int get_block_size() const { return block_size; };
	int blocks(int a) const { return max_blocks; }
	inline int operator ()(const long long& a, const long long& block_number, const int& blocks) const { return (a >> (block_size * block_number)) & ((1 << block_size) - 1); };
};

class string_extractor
{
	static const int block_size = 8;
public:
	int blocks(const std::string& s) const { return s.size(); };
	int get_block_size() const { return block_size; };
	inline int operator () (const std::string& s, int& block_number, const int& blocks) const
	{
		if ((blocks - block_number - 1) >= s.size()) 
			return 0;
		else
			return s[(blocks - block_number - 1)];
	};
};

class timer
{
	clock_t start_time;
	public:
		timer()
		{
			start_time = clock();
		}
		void start() 
		{ 
			start_time = clock(); 
		}
		double get_time() 
		{
			return (clock() - start_time) / (double) CLOCKS_PER_SEC;
		}
};

template <typename It, class BlockExtractor>
void digit_sort(It l, It r, const BlockExtractor& extr)
{
	typedef typename std::iterator_traits<It>::value_type value_type;
	typedef value_type* pvalue_type;
	int size = r - l;
	int count_size = (1 << extr.get_block_size()) + 1;
	int blocks = 0;
	for (int i = 0; i < size; i ++)
		blocks = std::max(blocks, extr.blocks(*(l + i)));
	std::vector <value_type> sorted_array(size);
	std::vector <int> count(count_size);
	for (int block_number = 0; block_number < blocks; block_number ++)
	{
		fill(count.begin(),count.end(),0);
		for (It i = l; i != r; i ++)
			count[extr(*i,block_number, blocks) + 1] ++;
		for (int i = 0; i < count_size - 1; i ++)
			count[i + 1] += count[i];
		for (It i = l; i != r; i ++)
			sorted_array[count[extr(*i, block_number, blocks)] ++] = *i;
		copy(sorted_array.begin(),sorted_array.end(),l);
	}
};


template <typename It>
bool is_sorted(It l, It r)
{
	It next = l; next++;
	for (It i = l; next != r; next ++, i ++)
		if (*next < *i)
			return false;
	return true;
};

template <typename It>
bool is_equal(It first, It second, int n)
{
	for (int i = 0; i < n; i ++)
		if (!(*(first + i) == *(second + i)))
			return false;
	return true;

}


void measure_int(int bits = 8, int n = 50000000)
{
	int* a = new int[n];
	int* b = new int[n];
	for (int i = 0; i < n; i ++)
		a[i] = b[i] = rand();
	timer t;
	t.start();
	std::sort(a,a + n);
	double std_sort_time = t.get_time();
	t.start();
	digit_sort(b,b + n, int_extractor(bits));
	double digit_sort_time = t.get_time();
	assert(is_sorted(b,b + n));
	assert(is_equal(a,b,n));
	fprintf(stderr, "%d integers. Extracting %d bits. std::sort works %.3lf seconds, my::digit_sort works %.3lf seconds. Factor is %.5lf\n", n, bits, std_sort_time, digit_sort_time, digit_sort_time/std_sort_time);
	fprintf(stdout, "%d integers. Extracting %d bits. std::sort works %.3lf seconds, my::digit_sort works %.3lf seconds. Factor is %.5lf\n", n, bits, std_sort_time, digit_sort_time, digit_sort_time/std_sort_time);
	delete[] a;
	delete[] b;
}

void measure_long_long(int bits = 8,int n = 50000000)
{
	long long* a = new long long[n];
	long long* b = new long long[n];
	for (int i = 0; i < n; i ++)
		a[i] = b[i] = ((((long long)rand()) << 32LL) | ((long long)rand()));
	timer t;
	t.start();
	std::sort(a,a + n);
	double std_sort_time = t.get_time();
	t.start();
	digit_sort(b,b + n, long_long_extractor(bits));
	double digit_sort_time = t.get_time();
	assert(is_sorted(b,b + n));
	assert(is_equal(a,b,n));
	fprintf(stderr, "%d long long integers. Extracting %d bits. std::sort works %.3lf seconds, my::digit_sort works %.3lf seconds. Factor is %.5lf\n", n, bits, std_sort_time, digit_sort_time, digit_sort_time/std_sort_time);
	fprintf(stdout, "%d long long integers. Extracting %d bits. std::sort works %.3lf seconds, my::digit_sort works %.3lf seconds. Factor is %.5lf\n", n, bits, std_sort_time, digit_sort_time, digit_sort_time/std_sort_time);
	delete[] a;
	delete[] b;
}

std::string gen_string(int len)
{
	std::string res(len,0);
	for (int i = 0; i < len; i ++)
		res[i] = rand() % 128;
	return res;
}

void measure_string(int len = 6, int n = 50000000)
{
	std::string* a = new std::string[n];
	std::string* b = new std::string[n];
	for (int i = 0; i < n; i ++)
		a[i] = b[i] = gen_string(len);
	timer t;
	t.start();
	digit_sort(b,b + n, string_extractor());
	double digit_sort_time = t.get_time();
	t.start();
	std::sort(a,a + n);
	double std_sort_time = t.get_time();
	assert(is_sorted(b,b + n));
	assert(is_equal(a,b,n));
	fprintf(stderr, "%d strings with length equal to %d. std::sort works %.3lf seconds, my::digit_sort works %.3lf seconds. Factor is %.5lf\n", n, len, std_sort_time, digit_sort_time, digit_sort_time / std_sort_time);
	fprintf(stdout, "%d strings with length equal to %d. std::sort works %.3lf seconds, my::digit_sort works %.3lf seconds. Factor is %.5lf\n", n, len, std_sort_time, digit_sort_time, digit_sort_time / std_sort_time);
	delete[] a;
	delete[] b;
}



int main()
{
	measure_int(8,5000000);
	return 0;
	for (int i = 1; i <= 6; i ++)
		measure_string(i);
	measure_long_long();
	measure_int();
	for (int i = 1; i <= 24; i ++)
		measure_int(i);
	return 0;
}
