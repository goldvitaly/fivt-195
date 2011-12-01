#include "sortfile.h"
#include <algorithm>
#include <fstream>
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

const int inf = 1<<30;

int ax[] = {0, 1, -1, 0, 0};
int ay[] = {0, 0, 0, -1, 1};

int main()
{
    //10^9 - 319 sec
    ofstream o11("input.txt");
    for (int i=1; i<=100000000; i++)
        o11<< rand()<<' ';
    int start = clock();
    string str = "input.txt";
    sort_file(str);
    cout<< clock() - start;

    return 0;
}
