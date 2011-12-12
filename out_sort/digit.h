#ifndef DIGIT_H_
#define DIGIT_H_

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
	explicit integral_extractor(int block_size_ = 8): block_size(block_size_), max_blocks(math::divide_and_round_up(bits, block_size_)) { };
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

template <class T, class Comparator = std::less<T> >
class integral_digit_sorter
{
  public:
	typedef Comparator comparator;
	Comparator comp;
	integral_extractor<T> extractor;
	explicit integral_digit_sorter(integral_extractor<T> extr, Comparator c = Comparator()): extractor(extr), comp(c)
	{
	}
	typedef T value_type;
	template <typename It> void operator() (It left, It right) const
	{
		digit_sort(left, right, extractor);
	}
};

#endif
