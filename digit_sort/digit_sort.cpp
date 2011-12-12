#include <vector>
#include <algorithm>
#include <string>
#include <time.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <limits> 
#include <numeric>

namespace math
{
	template <class T> T divide_and_round_up(T a, T b) { return a / b + bool(a % b); };
};



template <class T>
class integral_extractor
{
	static const int bits = sizeof(T) * 8; // or numeric_limits<T>::digits ?;
	const int block_size;
	const int max_blocks;
public:
	integral_extractor(int block_size_ = 8): block_size(block_size_), max_blocks(math::divide_and_round_up(bits, block_size_)) { };
	int get_block_size() const  { return block_size; };
	int blocks_in_element(T element) const { return max_blocks; }
	int operator ()(T element, int block_number, int blocks) const 
	{ 
		return (element >> (block_size * block_number)) & ((1 << block_size) - 1); 
	};
};

class string_extractor
{
	static const int block_size = 8;
public:
	int blocks_in_element(const std::string& element) const { return element.size(); };
	int get_block_size() const { return block_size; };
	int operator () (const std::string& s, int& block_number, const int& blocks) const
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
		double get_time() const
		{
			return (clock() - start_time) / (double) CLOCKS_PER_SEC;
		}
};

template <typename It, class BlockExtractor>
void digit_sort(It begin, It end, const BlockExtractor& extr)
{
	typedef typename std::iterator_traits<It>::value_type value_type;
	typedef value_type* pvalue_type;
	int size = end - begin;
	int number_of_blocks = 0;
	for (int i = 0; i < size; i++)
		number_of_blocks = std::max(number_of_blocks, extr.blocks_in_element(*(begin + i)));
	std::vector <value_type> sorted_array(size);
	int count_size = (1 << extr.get_block_size()) + 1;
	std::vector <unsigned int> count(count_size);
	for (int block_number = 0; block_number < number_of_blocks; block_number++)
	{
		fill(count.begin(),count.end(),0);
		for (It i = begin; i != end; i++)
			count[extr(*i,block_number, number_of_blocks) + 1]++;
		for (int i = 0; i < count_size - 1; i++)
			count[i + 1] += count[i];
		for (It i = begin; i != end; i++)
		{
			unsigned int& position = count[extr(*i, block_number, number_of_blocks)];
			sorted_array[position++] = *i;
		}
		copy(sorted_array.begin(),sorted_array.end(),begin);
	}
};

template <typename It, class Comp = std::less< typename std::iterator_traits<It>::value_type > >
bool is_sorted(It begin, It end, Comp comparator)
{
	if (begin == end) return true;
	It next = begin; next++;
	for (It i = begin; next != end; next++, i++)
		if (comparator(*next,*i))
			return false;
	return true;
};

void measure_int(int bits = 8, int n = 50000000)
{
	std::vector<int> a(n);
	std::vector<int> b(n);
	for (int i = 0; i < n; i++)
		a[i] = b[i] = rand();
	timer t;
	t.start();
	std::sort(a.begin(),a.end());
	double std_sort_time = t.get_time();
	t.start();
	digit_sort(b.begin(),b.end(), integral_extractor<int>(bits));
	double digit_sort_time = t.get_time();
	assert(is_sorted(b.begin(),b.end()));
	assert(std::equal(a.begin(),a.end(),b.begin()));
	fprintf(stderr, "%d integers. Extracting %d bits. std::sort works %.3lf seconds, my::digit_sort works %.3lf seconds. Factor is %.5lf\n", n, bits, std_sort_time, digit_sort_time, digit_sort_time/std_sort_time);
	fprintf(stdout, "%d integers. Extracting %d bits. std::sort works %.3lf seconds, my::digit_sort works %.3lf seconds. Factor is %.5lf\n", n, bits, std_sort_time, digit_sort_time, digit_sort_time/std_sort_time);
}

void measure_long_long(int bits = 8,int n = 50000000)
{
	std::vector<long long> a(n);
	std::vector<long long> b(n);
	for (int i = 0; i < n; i++)
		a[i] = b[i] = ((((long long)rand()) << 32LL) | ((long long)rand()));
	timer t;
	t.start();
	std::sort(a.begin(),a.end());
	double std_sort_time = t.get_time();
	t.start();
	digit_sort(b.begin(),b.end(), integral_extractor<long long>(bits));
	double digit_sort_time = t.get_time();
	assert(is_sorted(b.begin(),b.end()));
	assert(std::equal(a.begin(),a.end(), b.begin()));
	fprintf(stderr, "%d long long integers. Extracting %d bits. std::sort works %.3lf seconds, my::digit_sort works %.3lf seconds. Factor is %.5lf\n", n, bits, std_sort_time, digit_sort_time, digit_sort_time/std_sort_time);
	fprintf(stdout, "%d long long integers. Extracting %d bits. std::sort works %.3lf seconds, my::digit_sort works %.3lf seconds. Factor is %.5lf\n", n, bits, std_sort_time, digit_sort_time, digit_sort_time/std_sort_time);
}

std::string gen_string(int len)
{
	std::string res(len,0);
	for (int i = 0; i < len; i++)
		res[i] = rand() % 128;
	return res;
}

void measure_string(int len = 6, int n = 50000000)
{
	std::vector<std::string> a(n);
	std::vector<std::string> b(n);
	for (int i = 0; i < n; i++)
		a[i] = b[i] = gen_string(len);
	timer t;
	t.start();
	digit_sort(b.begin(),b.end(), string_extractor());
	double digit_sort_time = t.get_time();
	t.start();
	std::sort(a.begin(),a.end());
	double std_sort_time = t.get_time();
	assert(is_sorted(b.begin(),b.end()));
	assert(std::equal(a.begin(),a.end(),b.begin()));
	fprintf(stderr, "%d strings with length equal to %d. std::sort works %.3lf seconds, my::digit_sort works %.3lf seconds. Factor is %.5lf\n", n, len, std_sort_time, digit_sort_time, digit_sort_time / std_sort_time);
	fprintf(stdout, "%d strings with length equal to %d. std::sort works %.3lf seconds, my::digit_sort works %.3lf seconds. Factor is %.5lf\n", n, len, std_sort_time, digit_sort_time, digit_sort_time / std_sort_time);
}



int main()
{
	measure_int(8,500000);
	measure_long_long(8,500000);
	return 0;
}
