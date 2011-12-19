#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "bheap.h"
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    BHeap<int> a, b;
    srand(234);
    vector<int> v, v2;
    cout<< "Please Wait 3-4 seconds:\n\n";
    for (int i=1; i<1000; i++)
    {
        int x = rand();
        v.push_back(x);
        a.Push(x);

        x = rand();
        v2.push_back(x);
        b.Push(x);

        sort(v.begin(), v.end());

        if (v[0] != a.Top())
        {
            cout<< "It was found wrong MIN";
            return 0;
        }
        if (a.Size() != v.size())
        {
            cout<< "It was found wrong SIZE";
            return 0;
        }
        if (rand() % 23 == 0)
        {
            a.PopTop();
            swap(v[0], v[v.size()-1]);
            v.pop_back();
        }
        if (rand() % 241 == 0)
        {
            a.Add(b);
            for (i=0; i<v2.size(); i++)
                v.push_back(v2[i]);
            v2.clear();
        }
    }
    cout<< "Congratulations!!! Your code is perfect.\n";

    return 0;

}
