#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
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


template <class It, class Cmp>
void DigitSort(It b, It e, Cmp p)
{
    size_t Length = e - b;
    size_t number_of_values = p.NumberOfBlockValues();
    size_t BlocksNum = p.GetBlocksNum();
    vector <size_t> temp;
    vector <typeof(*b)> sorted;
    temp.resize(number_of_values);
    sorted.resize(Length);
    for(size_t i = 0; i < BlocksNum; i++)
    {
        for(size_t j = 0; j < number_of_values; j++)  temp[j] = 0;
        for(size_t j = 0; j < Length; j++)  temp[p.Extract(*(b + j), i)]++;
        for(size_t j = 1; j < number_of_values; j++)   temp[j] += temp[j - 1];
        for(size_t j = number_of_values - 1; j > 0; j--)   temp[j] = temp[j - 1];
        temp[0] = 0;
        for(size_t j = 0; j < Length; j++)
            sorted[temp[p.Extract(*(b + j), i)]++] = *(b + j);
        for(size_t j = 0; j < Length; j++)
            *(b + j) = sorted[j];
    }
    temp.clear();
    sorted.clear();
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
    last = clock();
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
