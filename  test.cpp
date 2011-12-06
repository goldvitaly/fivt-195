#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "IntervalTree.h"
#include "SlowlyTree.h"

using namespace std;

struct Element{
    int l, r;
    int valSum, valMin;
    explicit Element(const int l_ = 0, const int r_ = 0, const int valSum_ = 0, const int valMin_ = 0)
    {
        l = l_;
        r = r_;
        valMin = valMin_;
        valSum = valSum_;
    }
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
            elem.valSum = modif.assign*(elem.r - elem.l + 1);
            elem.valMin = modif.assign;
        }
        else
        {
            elem.valSum += modif.add*(elem.r - elem.l + 1);
            elem.valMin += modif.add;
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
        res.valSum = elem1.valSum + elem2.valSum;
        res.valMin = min(elem1.valMin, elem2.valMin);
        return res;
    }
};

int main()
{
    srand(time(NULL));
    vector<Element> Data(100);
    for(int i = 0; i < 100; i++)
    {
        Data[i] = Element(i, i, 0, 0);
    }
    IntervalTree<Element, Modif, Use, Conslol, ComposMod> intervalTree(100, Modif(), Data);
    SlowlyTree  slowlyTree(100);
    for(size_t i = 0; i < 500; i++)
    {
        int limit1 = rand() % 100 + 1;
        int limit2 = rand() % 100 + 1;
        if(limit2 < limit1)
            swap(limit1, limit2);
        if(limit1 != limit2)
        {
            int mod = rand() % 100 - 50;
            int choose = rand() % 3;
            if(choose == 0)         // +=
            {
                intervalTree.update(Modif(false, 0, mod), limit1, limit2);
                slowlyTree.update_add(mod, limit1, limit2);
            }
            else if(choose == 1)    // :=
            {
                intervalTree.update(Modif(true, mod, 0), limit1, limit2);
                slowlyTree.update_assign(mod, limit1, limit2);
            }
            else
            {
                Element elemIntTree = intervalTree.query(limit1, limit2);
                if(elemIntTree.valSum != slowlyTree.query_sum(limit1, limit2))
                    exit(1);
                if(elemIntTree.valMin != slowlyTree.query_min(limit1, limit2))
                    exit(1);
            }
        }
        else
            i--;
    }
    cout << "Ok" << endl;
   return 0;
}
