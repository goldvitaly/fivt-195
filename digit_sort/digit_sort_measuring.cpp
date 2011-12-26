#include <cassert>
#include <numeric>
#include <typeinfo>
#include <cassert>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include <vector>

#include "util.h"

using namespace std;

/* Задача: написать функцию цифровой сортировки (например, digit_sort(begin, end, BlockExtr))
 * Уметь сортировать:
 * - пары чисел
 * - строки одинаковой длины (произвольные строки - чуть сложнее)
 * - маленькие числа - за небольшое число памяти
 * - большое количество чисел - быстрее std::sort
 * - и вообще произвольные структуры, у которых мы можем получать значение блока по индексу
 */

//так более пафосно просто.
  /**
   * @brief Digitally sort range [begin, end)
   * @param	begin		Start of range to sort.
   * @param end			End of range to sort
   * @param Extractor	(const OT& object, size_t BlockNumber) -> size_t Block
   * @return void
  */
  
  /*template<typename T>
  struct IteratorTraits;
  
  template<>
  struct IteratorTraits< vector<int>::iterator > {
	  typedef int element_type;
  };*/
  
template<class Iterator, class BlockExtractor>
void digit_sort(Iterator begin, Iterator end, BlockExtractor Extractor, size_t ExplicitStepsNumber = 100500)
{
	enum
	{
		DefaultBlockSize = 260
	};
	vector<size_t> blockAmount(DefaultBlockSize);
	vector<typeof *begin> mirror(end-begin); //TODO: how to avoid typeof *begin?
	//vector<typename Iterator :: value_type> mirror(end-begin);
	bool StopFlag = false;
	
	for (size_t step = 0; !StopFlag && step < ExplicitStepsNumber; ++step)
	{
		fill(blockAmount.begin(), blockAmount.end(), 0);
		StopFlag = true;
		for (Iterator it = begin; it != end; ++it)
		{
			int ExtractedValue = Extractor(*it, step);
			if (ExtractedValue == -1)
				ExtractedValue = 0;
			else
				StopFlag = false;
			ExtractedValue++;
			if (ExtractedValue > (int)(blockAmount.size()))
				blockAmount.resize(ExtractedValue + 3);
			++ blockAmount[ExtractedValue];
		}
		//for (vector<size_t> :: iterator it = blockAmount.begin(); it != blockAmount.end() - 1; ++it)
//			*(it + 1) += *it;
		std :: partial_sum(blockAmount.begin(), blockAmount.end(), blockAmount.begin());
		
		for (Iterator it = begin; it != end; ++ it)
		{
			int ExtractedValue = Extractor(*it, step);
			if (ExtractedValue == -1)
				ExtractedValue = 0;
			mirror[blockAmount[ExtractedValue]] = *it;
			blockAmount[ExtractedValue] ++;
		}
		
		copy(mirror.begin(), mirror.end(), begin);
	}
}

template<class Iterator>
bool is_sorted (Iterator begin, Iterator end)
{
	Iterator it1 = begin;
	++it1;
	while (it1 != end)
	{
		if (*begin > *it1)
			return false;
		++it1;
	}
	return true;
}
template<class T, class Extractor, class T_Gen>
void measure(Extractor BlockExtractor, T_Gen Gen, const string& tname = "my class", int step = 100000)
{	
	
	const int maxn = 5000000,
			  seed = 991845232;
	clock_t start;
	
	T *d = new T[maxn];
	
	srand(seed);
	generate(d, d+maxn, Gen);
	start = clock();
	sort(d, d+maxn);
	/*if (!is_sorted(d, d+maxn))
	{
		cout << "std :: sort failed\n";
		assert(0);
	}*/
	long long std_time = (clock() - start) / 1000;
	cout << "std :: sort needs   " <<  std_time << " ms to sort " << maxn << " objects of type " << tname << ".\n";
	
	srand(seed);
	generate(d, d+maxn, Gen);
	start = clock();
	digit_sort(d, d+maxn, BlockExtractor, step);
	/*if (!is_sorted(d, d+maxn))
	{
		cout << "My sort failed\n";
		assert(0);
	}*/
	long long my_time = (clock() - start) / 1000;
	cout << "my digit sort needs " << my_time << " ms to sort " << maxn << " objects of type " << tname << ".\n";
	
	cout << endl;
	
	delete[] d;
}
int main()
{
	measure<int>(intExtractor, rand, "int", 4);
	measure<long long>(i64Extractor, i64Rand, "long long", 8);
	measure<string>(stringExtractor<5>, strRand<5>, "string of length 5", 5);
	measure<pair<int, int> >(pairExtractor, pairRand, "pair<int, int>", 8);

	return 0;
}

