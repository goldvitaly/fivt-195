#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "IntervalTree.h"

using namespace std;

struct Modif{
    bool assingExists;
    int assign;
    int add;
    explicit Modif(const bool assignExists_ = false, const int assign_ = 0, const int add_ = 0)
    {
        assingExists = assignExists_;
        assign = assign_;
        add = add_;
    }
};

class Min{
public:
    int operator()(const int val1, const int val2) const
    {
        return min(val1, val2);
    }
};

class ComposMod{
public:
    Modif operator()(Modif early, const Modif& late) const
    {
        if(late.assingExists)
        {
            early = Modif(true, late.assign, 0);
        }
        else
        {
            if(early.assingExists)
            {
                early.assign += late.add;
            }
            else
            {
                early.add += late.add;
            }
        }
        return early;
    }
};

class Use{
public:
    int operator()(const int val, const Modif& modif) const
    {
        if(modif.assingExists)
        {
            return modif.assign;
        }
        else
        {
            return val + modif.add;
        }
    }
};

class SlowlyTree{
    vector<int> Data;
    int sizeTree;
public:
    SlowlyTree(const int sizeTree_)
    {
        sizeTree = sizeTree_;
        Data.resize(sizeTree);
    }
    void update_add(const int mod, const int l, const int r)
    {
        for(int i = l - 1; i < r; i++)
            Data[i] += mod;
    }
    void update_assign(const int mod, const int l, const int r)
    {
        for(int i = l - 1; i < r; i++)
            Data[i] = mod;
    }
    int query(const int l, const int r)
    {
        int ans = Data[l-1];
        for(int i = l; i < r; i++)
            ans = min(ans, Data[i]);
        return ans;
    }
};

int main()
{
    srand(time(NULL));
    IntervalTree<int, Modif, Use, Min, ComposMod> intervalTree(100, Modif(), 0);
    SlowlyTree  slowlyTree(100);
    for(size_t i = 0; i < 500; i++)
    {
        int limit1 = rand() % 100 + 1;
        int limit2 = rand() % 100 + 1;
        if(limit1 != limit2)
        {
            int mod = rand() % 100 - 50;
            int choose = rand() % 3;
            if(choose == 0)         // +=
            {
                intervalTree.update(Modif(false, 0, mod), min(limit1, limit2), max(limit1,limit2));
                slowlyTree.update_add(mod, min(limit1, limit2), max(limit1,limit2));
            }
            else if(choose == 1)    // :=
            {
                intervalTree.update(Modif(true, mod, 0), min(limit1, limit2), max(limit1,limit2));
                slowlyTree.update_assign(mod, min(limit1, limit2), max(limit1,limit2));
            }
            else
            {
                if(intervalTree.query(min(limit1, limit2), max(limit1,limit2))
                    != slowlyTree.query(min(limit1, limit2), max(limit1,limit2)))
                    exit(1);
            }
        }
        else
            i--;
    }
    cout << "Ok" << endl;
    return 0;
}
