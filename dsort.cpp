#include <iostream>
#include <vector>
#include <cassert>
#include <typeinfo>
#include <ctime>
#include <algorithm>
#include <cstdio>

using namespace std;

//typedef string my_type;
//const int SIZE = 10 * 1000 * 1000;

class IKeyExtractor
{
public:
    size_t blockNum;
    
    IKeyExtractor(size_t KeyBlockNum = 2)
    {
        blockNum = KeyBlockNum;
    }
    size_t operator () (int Source, size_t KeyIndex)
    {
        unsigned short int *a = (unsigned short int *)(&Source);
        return *(a + KeyIndex);
    }
};

class LLKeyExtractor
{
public:
    size_t blockNum;
    
    LLKeyExtractor(size_t KeyBlockNum = 4)
    {
        blockNum = KeyBlockNum;
    }
    size_t operator () (long long Source, size_t KeyIndex)
    {
        unsigned short int *a = (unsigned short int *)(&Source);
        return *(a + KeyIndex);
    }
};

class SKeyExtractor
{
public:
    size_t blockNum;
    
    SKeyExtractor(size_t KeyBlockNum = 2)
    {
        blockNum = KeyBlockNum;
    }
    size_t operator () (string &Source, size_t KeyIndex)
    {
        return ((size_t)(Source[blockNum * 2 - 1 - KeyIndex * 2]) + (size_t(Source[blockNum * 2 - 1 - KeyIndex * 2 - 1]) << 8));
    }
};

class PLLKeyExtractor
{
public:
    size_t blockNum;
    
    PLLKeyExtractor(size_t KeyBlockNum = 8)
    {
        blockNum = KeyBlockNum;
    }
    size_t operator () (pair<long long, long long> Source, size_t KeyIndex)
    {
        unsigned short int *a = (unsigned short int *)(&Source);
        return *(a + KeyIndex);
    }
};

template <typename T, typename EXT>
void DigitalSort (T Begin, T End, EXT KE )
{
    const size_t KeyBlockSize = (1<<16), ArrayLength = End - Begin;
    
    vector <__typeof(*Begin)> Buffer(ArrayLength);
    vector <size_t> Num;
    
    for (size_t Iteration = 0; Iteration < KE.blockNum; ++Iteration)
    {
        Num.resize(0);
        Num.resize(KeyBlockSize, 0);
        
        for (T Iterator = Begin; Iterator != End; ++Iterator)
        {
            Num[KE(*Iterator, Iteration)]++;
        }
        
        size_t Sum = 0;
        for (size_t i = 0; i < KeyBlockSize; ++i)
        {
            swap( Sum, Num[i] );
            Sum += Num[i];
        }
        
        for (T Iterator = Begin; Iterator != End; ++Iterator)
        {
            Buffer[Num[KE(*Iterator, Iteration)]++] = *Iterator;
        }
        
        for (size_t i = 0; i < ArrayLength; ++i)
        {
            Begin[i] = Buffer[i];
        }
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

string rand_string ()
{
    string s = "";
    for (int i = 0; i < 5; ++i)
        s += ('a' + rand() % 20);
    return s;
}

pair<long long, long long> rand_pair_long_long ()
{
    return make_pair( rand(), rand() );
}

double dt (time_t a, time_t b)
{
    return double(b - a) / CLOCKS_PER_SEC;
}

vector <int> vi;
vector <long long> vll;
vector <string> vs;
vector < pair<long long, long long> > vpll;

int main ()
{
    return 0;
}
