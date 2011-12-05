// Одилзода Мехрдод 195 группа, МФТИ
// биномиальная куча, задание №3
#include "bheap.h"
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

#define mp make_pair
#define f first
#define s second
#define pb push_back

using namespace std;

typedef long long lglg;
typedef double db;

const int inf = 1<<30, N = 1002;

int ax[] = {0, 1, -1, 0, 0};
int ay[] = {0, 0, 0, -1, 1};


int main()
{
    BHeap<int> a, b;
    srand(532454);
    for (int i=0; i<10; i++)
    {
        int x = -rand();
        cout<< x<<' ';
        a.Push(x);
    }

    cout<< "\n\n";

    for (int i=0; i<10; i++)
    {
        int x = -rand() + 100000;
        cout<< x<<' ';
        b.Push(x);
    }
    cout<< "\n\n";
    cout<< a.Top()<<' ';
    a.Push(a.Top()-1);
    cout<< a.Top();
    return 0;
}
