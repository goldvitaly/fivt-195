#include "radix_sort.h"
#include "collection_Ext.h"
#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

template<class Type, class Iter, class Ext>
void cmp_algo_sort(Iter leftRadix, Iter rightRadix, Iter leftStd, Iter rightStd, Ext ext)
{
    srand(time(NULL));
    double START, FINISH;
    START = clock();
    radix_sort<Type>(leftRadix, rightRadix, ext);
    FINISH = clock();
    cerr.precision(10);
    cerr << "n = " << rightRadix - leftRadix << endl;
    cerr << "radix_sort = " << (FINISH - START) / CLOCKS_PER_SEC << endl;
    START = clock();
    sort(leftStd, rightStd);
    FINISH = clock();
    cerr << "stl_sort =   " << (FINISH - START) / CLOCKS_PER_SEC << endl;
    if(equal(leftRadix, rightRadix, leftStd) == false)
        exit(1);
}

void test_int(int n, int bit = 8)
{
    typedef unsigned int UInt;
    srand(time(NULL));
    cerr << "test int:" << endl;
    vector<UInt> A(n);
    vector<UInt> B(n);
    for(int i = 0; i < n; i++)
    {
        A[i] = (rand() << 16) + rand();
    }
    B = A;
    cmp_algo_sort<UInt>(A.begin(), A.end(), B.begin(), B.end(), IntegerBitExt<UInt>(bit));
}

void test_long_long(int n, int bit = 8)
{
    typedef unsigned long long ULongLong;
    srand(time(NULL));
    cerr << "test long long:" << endl;
    vector<ULongLong> A(n), B(n);
    for(int i = 0; i < n; i++)
    {
        A[i] = ((ULongLong)rand() << 48) + ((ULongLong)rand() << 32) + ((ULongLong)rand() << 16) + rand();
    }
    B = A;
    cmp_algo_sort<ULongLong>(A.begin(), A.end(), B.begin(), B.end(), IntegerBitExt<ULongLong>(bit));
}

void test_pair(int n, int bit = 8)
{
    typedef unsigned int UInt;
    srand(time(NULL));
    cerr << "test pair<int, int>:" << endl;
    vector<pair<UInt, UInt> > A(n), B(n);
    for(int i = 0; i < n; i++)
    {
        A[i].first = ((UInt)rand() << 16) + rand();
        A[i].second = ((UInt)rand() << 16) + rand();
    }
    B = A;
    cmp_algo_sort<pair<UInt, UInt> >(A.begin(), A.end(), B.begin(), B.end(), PairBitExt(bit));
}

void test_string(int n, int len = 5, int bits = 1)
{
    srand(time(NULL));
    cerr << "test string:" << endl;
    vector<string> A(n), B(n);
    string sim = "qwertyuiopasdfghjklzxcvbnm";
    for(int i = 0; i < n; i++)
    {
        A[i].resize(len);
        for(int j = 0; j < len; j++)
            A[i][j] = sim[rand()%26];
    }
    B = A;
    cmp_algo_sort<string>(A.begin(), A.end(), B.begin(), B.end(), SubStrExt(len, bits));
}

int main()
{
    int n = 10000000; //10**7
    int m, k;
    test_int(n, 8);
    test_long_long(n, 8);
    test_pair(n, 8);
    test_string(2*n/5, 3, 2);
}
