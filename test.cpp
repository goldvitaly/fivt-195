#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "IntervalTree.h"

using namespace std;

class Min{
public:
    int operator()(int val1, int val2) const
    {
        return min(val1, val2);
    }
};

class Plus{
public:
    int operator()(int val, int add) const
    {
        return val + add;
    }
};

class SlowlyTree{
    vector<int> Data;
    int sizeTree;
public:
    SlowlyTree(int sizeTree_)
    {
        sizeTree = sizeTree_;
        Data.resize(sizeTree);
    }
    void update(int mod, int l, int r)
    {
        for(int i = l-1; i < r; i++)
            Data[i] += mod;
    }
    int query(int l, int r)
    {
        int ans = Data[l-1];
        for(int i = l; i < r; i++)
            ans = min(ans, Data[i]);
        return ans;
    }
};

int main()
{
    IntervalTree<int, int, Plus, Min, Plus> intervalTree(100, Plus(), Min(), Plus(), 0, 0);
    SlowlyTree  slowlyTree(100);
    for(int i = 0; i < 100; i++)
    {
        srand(time(NULL));
        int limit1 = rand() % 100 + 1;
        int limit2 = rand() % 100 + 1;
        int mod = rand() % 100 - 50;
        if(limit1 != limit2)
        {
            intervalTree.update(mod, min(limit1, limit2), max(limit1,limit2));
            slowlyTree.update(mod, min(limit1, limit2), max(limit1,limit2));
        }
        else
            i--;
    }
    for(int i = 0; i < 100; i++)
    {
        srand(time(NULL));
        int limit1 = rand() % 100 + 1;
        int limit2 = rand() % 100 + 1;
        if(limit1 != limit2)
        {
            if(intervalTree.query(min(limit1, limit2), max(limit1,limit2))
                != slowlyTree.query(min(limit1, limit2), max(limit1,limit2)))
                exit(1);
        }
        else
            i--;
    }
    cout << "Ok" << endl;
    return 0;
}
