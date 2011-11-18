#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>
#include <math.h>
#include <set>
#include <map>

#define forn(i, n) for(int i = 0; i < n; i++)
#define VI vector<int>
#define PII pair<int, int>
#define fi first
#define se second
#define mp(a, b) make_pair(a, b)
#define pb(a) push_back(a)
#define deb(a); cout << #a << " = " << a << endl;
#include <fstream>
using namespace std;

const int maxn = 500000;
const long long inf = 10000000000000000LL;
const double eps = 1.0e-7;

int main()
{
//    freopen("input.txt", "r", stdin);
//    freopen("output.txt", "w", stdout);
    for(int i = 0; i < 10; i++)
    {
        char A1[20];
        char A2[20];
        sprintf(A1, "%d.a", i);
        sprintf(A2, "%d.a2", i);
        ifstream fin1(A1);
        ifstream fin2(A2);
        while(!fin1.eof() && !fin2.eof())
        {
            int n1, n2;
            fin1 >> n1;
            fin2 >> n2;
            if(n1 != n2)
            {
                cout << "Not " << i << " " << n1 << " " << n2 << endl;
                return 1;
            }
        }
        if(!fin1.eof() || !fin2.eof())
        {
            cout << "Not2" << endl;
            return 1;
        }
    }
    cout << "Ok" << endl;
    return 0;
}
