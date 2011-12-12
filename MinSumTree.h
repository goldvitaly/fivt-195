#ifndef MINSUMTREE_H_INCLUDED
#define MINSUMTREE_H_INCLUDED

#include "IntervalTree.h"
#include "SlowlyTree.h"
#include <algorithm>
#include <vector>


struct Element{
    int l, r;
    int valSum, valMin;
    Element()
    {
        l = r = valSum = valMin = 0;
    }
    Element(const int l_, const int r_, const int valSum_, const int valMin_)
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
    Modif()
    {
        assingExists = false;
        assign = 0;
        add = 0;
    }
    Modif(const bool assignExists_, const int assign_, const int add_)
    {
        assingExists = assignExists_;
        assign = assign_;
        add = add_;
    }
};

class UseModif{
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

class Consolid{
public:
    Element operator()(const Element& elem1, const Element& elem2) const{
        Element res;
        res.l = elem1.l;
        res.r = elem2.r;
        res.valSum = elem1.valSum + elem2.valSum;
        res.valMin = std::min(elem1.valMin, elem2.valMin);
        return res;
    }
};

class MinSumTree{
    IntervalTree<Element, Modif, UseModif, Consolid, ComposMod> intervalTree;
public:
    explicit MinSumTree(int sizeTree):intervalTree()
    {
        std::vector<Element> Data;
        for(int i = 0; i < sizeTree; i++)
        {
            Data.push_back(Element(i, i, 0, 0));
        }
        intervalTree = IntervalTree<Element, Modif, UseModif, Consolid, ComposMod>(sizeTree, Modif(), Data);
    }
    void update_assign(int mod, int l, int r)
    {
        intervalTree.update(Modif(true, mod, 0), l, r);
    }
    void update_add(int mod, int l, int r)
    {
        intervalTree.update(Modif(false, 0, mod), l, r);
    }
    int query_min(int l, int r)
    {
        return intervalTree.query(l, r).valMin;
    }
    int query_sum(int l, int r)
    {
        return intervalTree.query(l, r).valSum;
    }
};

#endif // MINSUMTREE_H_INCLUDED
