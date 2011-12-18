#include <iostream>
#include <vector>
#include <cassert>
#include <typeinfo>
#include <ctime>
#include <algorithm>
#include <cstdio>
#include <numeric>

template <typename NumberType>
class NumberKeyExtractor
{
private:
    size_t blockNum;
public:   
    explicit NumberKeyExtractor(size_t KeyBlockNum = 2)
    {
        blockNum = KeyBlockNum;
    }
    size_t operator () (const NumberType &Source, size_t KeyIndex)const
    {
        unsigned short int *a = (unsigned short int *)(&Source);
        return *(a + KeyIndex);
    }
    size_t GetBlockNum ()const
	{
		return blockNum;
	}
};

class StringKeyExtractor
{
private:
    size_t blockNum;
public:
    explicit StringKeyExtractor(size_t KeyBlockNum = 2)
    {
        blockNum = KeyBlockNum;
    }
    size_t operator () (const std::string &Source, size_t KeyIndex)const
    {
        return ((size_t)(Source[blockNum * 2 - 1 - KeyIndex * 2]) + (size_t(Source[blockNum * 2 - 1 - KeyIndex * 2 - 1]) << 8));
    }
    size_t GetBlockNum ()const
    {
		return blockNum;
    }
};

template <typename iterator, typename Extractor>
void DigitalSort (iterator Begin, iterator End, Extractor KeyExtractor )
{
    const size_t KeyBlockSize = (1<<16), ArrayLength = End - Begin;
    
    std::vector <__typeof(*Begin)> Buffer(ArrayLength);
    std::vector <size_t> Num, StartIndex;
    
    for (size_t Iteration = 0; Iteration < KeyExtractor.GetBlockNum(); ++Iteration)
    {
        Num.resize(0);
        Num.resize(KeyBlockSize, 0);
        
        for (iterator Iterator = Begin; Iterator != End; ++Iterator)
        {
            Num[KeyExtractor(*Iterator, Iteration)]++;
        }
        
        //std::partial_sum(Num.begin(), Num.end() - 1, Num.begin() + 1);
        //Не работает, а использовать лишнюю линюю памяти я не хочу.
        size_t Sum = 0;
        for (size_t i = 0; i < KeyBlockSize; ++i)
        {
            std::swap( Sum, Num[i] );
            Sum += Num[i];
        }
        
        for (iterator Iterator = Begin; Iterator != End; ++Iterator)
        {
            Buffer[Num[KeyExtractor(*Iterator, Iteration)]++] = *Iterator;
        }
        
        std::copy(Buffer.begin(), Buffer.end(), Begin);
    }
}

int rand_int ()
{
    return rand();
}

long long rand_long_long ()
{
    return rand();
}

std::string rand_string ()
{
    std::string s = "";
    for (int i = 0; i < 6; ++i)
        s += ('a' + rand() % 20);
    return s;
}

std::pair<long long, long long> rand_pair_long_long ()
{
    return std::make_pair( rand(), rand() );
}

double GetDeltaTime (time_t a, time_t b)
{
    return double(b - a) / CLOCKS_PER_SEC;
}

bool mycmp (const std::pair<long long, long long> &a, const std::pair<long long, long long> &b)
{
	if (a.second != b.second) return a.second < b.second;
	if (a.first != b.first) return a.first < b.first;
	return false;
}

const int TESTLEN = 10000;

std::vector <int> vi(TESTLEN);
std::vector <long long> vll(TESTLEN);
std::vector <std::string> vs(TESTLEN);
std::vector < std::pair<long long, long long> > vpll(TESTLEN);

int main ()
{
	generate(vi.begin(), vi.end(), rand_int);
	generate(vll.begin(), vll.end(), rand_long_long);
	generate(vs.begin(), vs.end(), rand_string);
	generate(vpll.begin(), vpll.end(), rand_pair_long_long);

	DigitalSort(vi.begin(), vi.end(), NumberKeyExtractor<int>(2));
	DigitalSort(vll.begin(), vll.end(), NumberKeyExtractor<long long>(4));
	DigitalSort(vs.begin(), vs.end(), StringKeyExtractor(3));
	DigitalSort(vpll.begin(), vpll.end(), NumberKeyExtractor<std::pair<long long, long long> >(sizeof(std::pair<long long, long long>) / 2));
	
	std::cout << "Sorting int is: ";
	if (prev_permutation(vi.begin(), vi.end()))
		std::cout << "FAIL\n";
	else
		std::cout << "OK\n";
	
	std::cout << "Sorting long long is: ";
	if (prev_permutation(vll.begin(), vll.end()))
		std::cout << "FAIL\n";
	else
		std::cout << "OK\n";
	
	std::cout << "Sorting string is: ";
	if (prev_permutation(vs.begin(), vs.end()))
		std::cout << "FAIL\n";
	else
		std::cout << "OK\n";
	
	std::cout << "Sorting pair of long long is: ";
	if (prev_permutation(vpll.begin(), vpll.end(), mycmp))
		std::cout << "FAIL\n";
	else
		std::cout << "OK\n";
    return 0;
}
