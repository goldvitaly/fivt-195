#include "radix_sort.h"
#include "collection_Ext.h"
#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

template<class Type, class Iter, class Ext>
void cmp_algo_sort(Iter L_radix, Iter R_radix, Iter L_std, Iter R_std, Ext ext)
{
    srand(time(NULL));
    double START, FINISH;
    START = clock();
    radix_sort<Type>(L_radix, R_radix, ext);
    FINISH = clock();
    cout.precision(10);
    cout << "n = " << R_radix - L_radix << endl;
    cout << "radix_sort = " << (FINISH - START) / CLOCKS_PER_SEC << endl;
    START = clock();
    sort(L_std, R_std);
    FINISH = clock();
    cout << "stl_sort =   " << (FINISH - START) / CLOCKS_PER_SEC << endl;
    if(equal(L_radix, R_radix, L_std) == false)
        exit(1);
}

void test_int(int n, int bit = 8)
{
    srand(time(NULL));
    cout << "test int:" << endl;
    vector<unsigned int> A(n);
    vector<unsigned int> B(n);
    for(int i = 0; i < n; i++)
    {
        A[i] = (rand() << 16) + rand();
    }
    B = A;
    cmp_algo_sort<unsigned int>(A.begin(), A.end(), B.begin(), B.end(), int_BitExt(bit));
}

void test_long_long(int n, int bit = 8)
{
    srand(time(NULL));
    cout << "test long long:" << endl;
    vector<unsigned long long> A(n), B(n);
    for(int i = 0; i < n; i++)
    {
        A[i] = ((unsigned long long)rand() << 48) + ((unsigned long long)rand() << 32) + ((unsigned long long)rand() << 16) + rand();
    }
    B = A;
    cmp_algo_sort<unsigned long long>(A.begin(), A.end(), B.begin(), B.end(), long_long_BitExt(bit));
}

void test_pair(int n, int bit = 8)
{
    srand(time(NULL));
    cout << "test pair<int, int>:" << endl;
    vector<pair<unsigned int, unsigned int> > A(n), B(n);
    for(int i = 0; i < n; i++)
    {
        A[i].first = ((unsigned int)rand() << 16) + rand();
        A[i].second = ((unsigned int)rand() << 16) + rand();
    }
    B = A;
    cmp_algo_sort<pair<unsigned int, unsigned int> >(A.begin(), A.end(), B.begin(), B.end(), pair_BitExt(bit));
}

void test_string(int n, int len = 5, int bits = 1)
{
    srand(time(NULL));
    cout << "test string:" << endl;
    vector<string> A(n), B(n);
    string sim = "qwertyuiopasdfghjklzxcvbnm";
    for(int i = 0; i < n; i++)
    {
        A[i].resize(len);
        for(int j = 0; j < len; j++)
            A[i][j] = sim[rand()%26];
    }
    B = A;
    cmp_algo_sort<string>(A.begin(), A.end(), B.begin(), B.end(), SubstrExt(len, bits));
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
