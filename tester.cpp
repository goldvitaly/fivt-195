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
    cout<< "Please Wait:\n\n";
    int cnt = 0;
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

            a.Merge(b);
            for (int j=0; j<v2.size(); j++)
                v.push_back(v2[j]);
            sort(v.begin(), v.end());
            v2.clear();
        }


    }
    cout<< "Random tests pasted\n\n";
    BHeap<int> na, nb;
    v.clear();a.Merge(b);
    v2.clear();
    for (int i=1; i<1000; i++)
    {
        int x = rand();
        v.push_back(x);
        na.Push(x);

        x = rand();
        v2.push_back(x);
        nb.Push(x);

        sort(v.begin(), v.end());

        if (v[0] != na.Top())
        {
            cout<< "It was found wrong MIN";
            return 0;
        }
        if (na.Size() != v.size())
        {
            cout<< "It was found wrong SIZE";
            return 0;
        }
        if (i % 20 == 0)
        {
            na.PopTop();
            swap(v[0], v[v.size()-1]);
            v.pop_back();
        }
        if (i % 40 == 0)
        {
            na.Merge(nb);
            for (int j=0; j<v2.size(); j++)
                v.push_back(v2[j]);
            v2.clear();
        }

    }

    cout<< "Congratulations!!! Your code is perfect.\n";

    return 0;

}
