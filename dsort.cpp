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

class SKeyExtractor
{
private:
    size_t blockNum;
public:
    explicit SKeyExtractor(size_t KeyBlockNum = 2)
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
void DigitalSort (iterator Begin, iterator End, Extractor KE )
{
    const size_t KeyBlockSize = (1<<16), ArrayLength = End - Begin;
    
    std::vector <__typeof(*Begin)> Buffer(ArrayLength);
    std::vector <size_t> Num, StartIndex;
    
    for (size_t Iteration = 0; Iteration < KE.GetBlockNum(); ++Iteration)
    {
        Num.resize(0);
        Num.resize(KeyBlockSize, 0);
        
        for (iterator Iterator = Begin; Iterator != End; ++Iterator)
        {
            Num[KE(*Iterator, Iteration)]++;
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
            Buffer[Num[KE(*Iterator, Iteration)]++] = *Iterator;
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
    for (int i = 0; i < 5; ++i)
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

std::vector <int> vi;
std::vector <long long> vll;
std::vector <std::string> vs;
std::vector < std::pair<long long, long long> > vpll;

int main ()
{
	std::pair<long long, long long> a[] = {std::make_pair(6, 0), std::make_pair(5, 0), std::make_pair(10, 0), std::make_pair(-177, 0), std::make_pair(454, 0), std::make_pair(00, 0), std::make_pair(157, 0)};
	DigitalSort(a, a + 7, NumberKeyExtractor<std::pair<long long, long long> >(8));
	for (int i = 0; i < 7; ++i)
		std::cout << a[i].first << " " << a[i].second << std::endl;
	
    return 0;
}
