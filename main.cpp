#include <iostream>
#include <functional>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <numeric>
#include <utility>
#include <cstdlib>
#include "BitExtractor.h"
using namespace std;




bool operator < (const pair <int, int> &x, const pair <int, int> &y)
{
    if(x.first == y.first)  return(x.second < y.second);
    return(x.first < y.first);
}


template<class It>
void CheckSortedVectors(It FirstBegin, It FirstEnd, It SecondBegin, It SecondEnd)
{
    bool fail = false;
    while(FirstBegin != FirstEnd  &&  SecondBegin != SecondEnd)
    {
        if(*FirstBegin != *SecondBegin) fail = true;
        FirstBegin++;
        SecondBegin++;
    }
    if(fail)    std::cout << "FAIL\n";
}


template <class It, class Ext>
void DigitSort(It b, It e, Ext p)
{
    size_t Length = e - b;
    size_t number_of_values = p.NumberOfBlockValues();
    size_t BlocksNum = p.GetBlocksNum();
    vector <size_t> storage;
    vector <typeof(*b)> sorted;
    storage.resize(number_of_values);
    sorted.resize(Length);
    for(size_t i = 0; i < BlocksNum; i++)
    {
        std::fill(storage.begin(), storage.end(), 0);
        for(size_t j = 0; j < Length; j++)  storage[p.Extract(*(b + j), i)]++;
        partial_sum(storage.begin(), storage.end(), storage.begin());
        for(size_t j = number_of_values - 1; j > 0; j--)   storage[j] = storage[j - 1];
        storage[0] = 0;
        for(size_t j = 0; j < Length; j++)
            sorted[storage[p.Extract(*(b + j), i)]++] = *(b + j);
        std::copy(sorted.begin(), sorted.end(), b);
    }
}

void test_int(int N)
{
    vector <int> sorted_by_stl, sorted_by_digit;
    time_t last;
    cout << "Ints:\n";
    sorted_by_stl.resize(N);
    sorted_by_digit.resize(N);
    for(int i = 0; i < N; sorted_by_stl[i] = rand(), sorted_by_digit[i] = sorted_by_stl[i], i++);
    last = clock();
    sort(sorted_by_stl.begin(), sorted_by_stl.end());
    cout << "  STL: " << clock() - last << endl;
    last = clock();
    DigitSort(sorted_by_digit.begin(), sorted_by_digit.end(), IntBitExtractor<int>(8));
    cout << "  MyDigit: " << clock() - last << endl;
    CheckSortedVectors(sorted_by_stl.begin(), sorted_by_stl.end(), sorted_by_digit.begin(), sorted_by_digit.end());
    sorted_by_stl.clear();
    sorted_by_digit.clear();
}


void test_long_long(int N)
{
    vector <long long> sorted_by_stl, sorted_by_digit;
    time_t last;
    cout << "Long Longs:\n";
    sorted_by_stl.resize(N);
    sorted_by_digit.resize(N);
    for(int i = 0; i < N; i++)
    {
        sorted_by_stl[i] = (long long)(rand()) * (long long)(rand());
        sorted_by_digit[i] = sorted_by_stl[i];
    }
    last = clock();
    sort(sorted_by_stl.begin(), sorted_by_stl.end());
    cout << "  STL: " << clock() - last << endl;
    last = clock();
    DigitSort(sorted_by_digit.begin(), sorted_by_digit.end(), IntBitExtractor<long long>(8));
    cout << "  MyDigit: " << clock() - last << endl;
    CheckSortedVectors(sorted_by_stl.begin(), sorted_by_stl.end(), sorted_by_digit.begin(), sorted_by_digit.end());
    sorted_by_stl.clear();
    sorted_by_digit.clear();
}


void test_pairs(int N)
{
    vector <pair<int, int> > sorted_by_stl, sorted_by_digit;
    time_t last;
    sorted_by_stl.resize(N);
    sorted_by_digit.resize(N);
    for(int i = 0; i < N; sorted_by_stl[i].first = rand(), sorted_by_stl[i].second = rand(),
        sorted_by_digit[i].first = sorted_by_stl[i].first, sorted_by_digit[i].second = sorted_by_stl[i].second, i++);
    last = clock();
    sort(sorted_by_stl.begin(), sorted_by_stl.end());
    cout << "Pairs:\n  STL: " << clock() - last << endl;
    last = clock();
    DigitSort(sorted_by_digit.begin(), sorted_by_digit.end(), PairBitExtractor::PairBitExtractor(8));
    cout << "  MyDigit: " << clock() - last << endl;
    CheckSortedVectors(sorted_by_stl.begin(), sorted_by_stl.end(), sorted_by_digit.begin(), sorted_by_digit.end());
    sorted_by_stl.clear();
    sorted_by_digit.clear();
}


void test_string(int N)
{
    vector <string> sorted_by_stl, sorted_by_digit;
    time_t last;
    sorted_by_stl.resize(N);
    sorted_by_digit.resize(N);
    cout << "Strings:\n";
    for(int i = 0; i < N; i++)
    {
        sorted_by_stl[i].clear();
        for(int j = 0; j < 5; j++)
            sorted_by_stl[i].push_back(rand() % 26 + 'a');
        sorted_by_digit[i] = sorted_by_stl[i];
    }
    last = clock();
    sort(sorted_by_stl.begin(), sorted_by_stl.end());
    cout << "  STL: " << clock() - last << endl;
    last = clock();
    DigitSort(sorted_by_digit.begin(), sorted_by_digit.end(), StringBitExtrator());
    cout << "  MyDigit: " << clock() - last << endl;
    CheckSortedVectors(sorted_by_stl.begin(), sorted_by_stl.end(), sorted_by_digit.begin(), sorted_by_digit.end());
    sorted_by_stl.clear();
    sorted_by_digit.clear();
}
int main()
{
    int N;
    cin >> N;
    srand(time(NULL));
    test_int(N);
    test_long_long(N);
    test_pairs(N);
    test_string(N);
    return 0;
}
