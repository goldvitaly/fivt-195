#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "IntervalTree.h"

using namespace std;

struct Elem{
    int l, r, val;
};

class ModInterval{
public:
    Elem operator()(Elem v, int add) const
    {
        v.val = v.val + add*(v.r - v.l + 1);
        return v;
    }
};

class Sum{
public:
    int operator()(int add1, int add2) const{

        return add1 + add2;
    }
};

class Conslol{
public:
    Elem operator()(Elem elem1, Elem elem2) const{
        Elem res;
        res.l = elem1.l;
        res.r = elem2.r;
        res.val = elem1.val + elem2.val;
        return res;
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
        int ans = 0;
        for(int i = l-1; i < r; i++)
            ans += Data[i];
        return ans;
    }
};

int main()
{
    vector<Elem> Data(100);
    for(int i = 0; i < 100; i++)
    {
        Data[i].l = i;
        Data[i].r = i;
        Data[i].val = 0;
    }
    IntervalTree<Elem, int, ModInterval, Conslol, Sum> intervalTree(100, 0, Data);
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
            if(intervalTree.query(min(limit1, limit2), max(limit1,limit2)).val
                != slowlyTree.query(min(limit1, limit2), max(limit1,limit2)))
                exit(1);
        }
        else
            i--;
    }
    cout << "Ok" << endl;
    return 0;
}
