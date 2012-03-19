#include <algorithm>
#include <iostream>
#include <memory.h>
#include <cstdlib>
#include <time.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <set>
#include <map>
#include "rsort.h"

#define mp make_pair
#define f first
#define s second
#define pb push_back

using namespace std;

typedef long long lglg;
typedef double db;


int ax[] = {0, 1, -1, 0, 0};
int ay[] = {0, 0, 0, -1, 1};
int N;

void test_int()
{
    vector<int> v1(N), v2(N);
    int i;
    for (i=0; i<N; i++)
        v2[i] = v1[i] = rand();
    time_t start = clock();
    sort(v1.begin(), v1.end());
    cout<< "STL: "<<clock() - start<<"  ";
    start = clock();
    RadixSort(v2.begin(), v2.end());
    cout<< "RadixSort: "<< clock() - start<<"\n\n";
    for (i=0; i<N; i++)
        if (v1[i] != v2[i])
            cout<< "WRONG SORT\n\n";
}

void test_long_long()
{
    vector<long long> v1(N), v2(N);
    int i;
    for (i=0; i<N; i++)
        v2[i] = v1[i] = rand();
    time_t start = clock();
    sort(v1.begin(), v1.end());
    cout<< "STL: "<<clock() - start<<"  ";
    start = clock();
    RadixSort(v2.begin(), v2.end());
    cout<< "RadixSort: "<< clock() - start<<"\n\n";
    for (i=0; i<N; i++)
        if (v1[i] != v2[i])
            cout<< "WRONG SORT\n\n";
}

int main()
{
    #ifndef ONLINE_JUDGE
        freopen("input.txt","r",stdin);
    #endif
    N = 50000000;
    cout<< "Number of elements: "<<N<<"\n\n";
    cout<< "Results for INT\n";
    test_int();
    cout<< "Results for LONG LONG\n";
    test_long_long();

    return 0;

}
