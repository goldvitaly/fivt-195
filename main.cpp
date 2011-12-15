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


vector <pair <int, int> > a, a2;
vector <string> s, s2;
vector <int> b, b2;
vector <long long> l, l2;
int N;

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

void test_int()
{
    time_t last;
    cout << "Ints:\n";
    b.resize(N);
    b2.resize(N);
    for(int i = 0; i < N; b[i] = rand(), b2[i] = b[i], i++);
    last = clock();
    sort(b.begin(), b.end());
    cout << "  STL: " << clock() - last << endl;
    last = clock();
    DigitSort(b2.begin(), b2.end(), IntBitExtractor<int>(8));
    cout << "  MyDigit: " << clock() - last << endl;
    CheckSortedVectors(b.begin(), b.end(), b2.begin(), b2.end());
    b.clear();
    b2.clear();
}


void test_long_long()
{
    time_t last;
    cout << "Long Longs:\n";
    l.resize(N);
    l2.resize(N);
    for(int i = 0; i < N; i++)
    {
        l[i] = (long long)(rand()) * (long long)(rand());
        l2[i] = l[i];
    }
    last = clock();
    sort(l.begin(), l.end());
    cout << "  STL: " << clock() - last << endl;
    last = clock();
    DigitSort(l2.begin(), l2.end(), IntBitExtractor<long long>(8));
    cout << "  MyDigit: " << clock() - last << endl;
    CheckSortedVectors(l.begin(), l.end(), l2.begin(), l2.end());
    l.clear();
    l2.clear();
}


void test_pairs()
{
    time_t last;
    a.resize(N);
    a2.resize(N);
    for(int i = 0; i < N; a[i].first = rand(), a[i].second = rand(),
        a2[i].first = a[i].first, a2[i].second = a[i].second, i++);
    last = clock();
    sort(a.begin(), a.end());
    cout << "Pairs:\n  STL: " << clock() - last << endl;
    last = clock();
    DigitSort(a2.begin(), a2.end(), PairBitExtractor::PairBitExtractor(8));
    cout << "  MyDigit: " << clock() - last << endl;
    CheckSortedVectors(a.begin(), a.end(), a2.begin(), a2.end());
    a.clear();
    a2.clear();
}


void test_string()
{
    time_t last;
    s.resize(N);
    s2.resize(N);
    cout << "Strings:\n";
    for(int i = 0; i < N; i++)
    {
        s[i].clear();
        for(int j = 0; j < 5; j++)
            s[i].push_back(rand() % 26 + 'a');
        s2[i] = s[i];
    }
    last = clock();
    sort(s.begin(), s.end());
    cout << "  STL: " << clock() - last << endl;
    last = clock();
    DigitSort(s2.begin(), s2.end(), StringBitExtrator());
    cout << "  MyDigit: " << clock() - last << endl;
    CheckSortedVectors(s.begin(), s.end(), s2.begin(), s2.end());
    s.clear();
    s2.clear();
}
int main()
{
    cin >> N;
    srand(time(NULL));
    test_int();
    test_long_long();
    test_pairs();
    test_string();
    return 0;
}
