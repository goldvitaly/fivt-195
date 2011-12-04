#include <iostream>
#include "IntervalTree.h"

using namespace std;

class consl{
public:
    int operator()(int val1, int val2) const
    {
        return min(val1, val2);
    }
};

class use{
public:
    int operator()(int val, int add) const
    {
        return val + add;
    }
};

class comp_add{
public:
    int operator()(int add1, int add2) const
    {
        return add1 + add2;
    }
};

int main()
{
    IntervalTree<int, int, use, consl, comp_add> MinInt(100, use(), consl(), comp_add(), 0, 0);
    cout << MinInt.size() << endl;
    MinInt.update(2, 1, 99);
    cout << MinInt.query(1, 100) << endl;
    cout << MinInt.query(1, 10)  << endl;
    MinInt.update(3, 1, 5);
    cout << MinInt.query(1, 7) << endl;
    cout << MinInt.query(2, 3) << endl;

    vector<int> Data(100);
    for(int i = 0; i < 100; i++)
    {
        Data[i] = i;
    }
    IntervalTree<int, int, use, consl, comp_add> MinInt2(100, use(), consl(), comp_add(), 0, Data);
    cout << MinInt2.query(33, 77) << endl;

    return 0;
}
