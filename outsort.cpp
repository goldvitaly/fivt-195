#include "sortfile.h"
#include <algorithm>
#include <fstream>
#include <memory.h>
#include <cstdlib>
#include <time.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <iostream>
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

const int inf = 1<<30;

int ax[] = {0, 1, -1, 0, 0};
int ay[] = {0, 0, 0, -1, 1};

int main()
{
    vector<int> v;
    ofstream o11("input.txt");
    for (int i=1; i<=1000000; i++)
    {
        int x = rand();
        v.push_back(x);
    }
    string str = "input.txt";
    sort_file(str);
    ifstream o22("result.txt");
    sort(v.begin(), v.end());
    int x, i = 0;
    while(o22>> x)
    {
        if (v[i] != x)
            cout<< "WRONG";
        i++;
    }
    cout<< "OK";

    return 0;
}
