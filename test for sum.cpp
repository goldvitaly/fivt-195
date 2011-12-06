#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "IntervalTree.h"

using namespace std;

struct Element{
    int l, r, val;
};

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

class Use{
public:
    Element operator()(Element elem, const Modif& modif) const
    {
        if(modif.assingExists)
        {
            elem.val = modif.assign*(elem.r - elem.l + 1);
        }
        else
        {
            elem.val += modif.add*(elem.r - elem.l + 1);
        }
        return elem;
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

class Conslol{
public:
    Element operator()(const Element& elem1, const Element& elem2) const{
        Element res;
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
    explicit SlowlyTree(const int sizeTree_)
    {
        sizeTree = sizeTree_;
        Data.resize(sizeTree);
    }
    void update_add(const int mod, const int l, const int r)
    {
        for(int i = l-1; i < r; i++)
            Data[i] += mod;
    }
    void update_assign(const int mod, const int l, const int r)
    {
        for(int i = l-1; i < r; i++)
            Data[i] = mod;
    }
    int query(const int l, const int r) const
    {
        int ans = 0;
        for(int i = l-1; i < r; i++)
            ans += Data[i];
        return ans;
    }
};

int main()
{
    srand(time(NULL));
    vector<Element> Data(100);
    for(int i = 0; i < 100; i++)
    {
        Data[i].l = i;
        Data[i].r = i;
        Data[i].val = 0;
    }
    IntervalTree<Element, Modif, Use, Conslol, ComposMod> intervalTree(100, Modif(), Data);
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
                if(intervalTree.query(min(limit1, limit2), max(limit1,limit2)).val
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
