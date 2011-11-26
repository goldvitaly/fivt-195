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

using namespace std;

template<class basic, class change>
class vertex{
    basic val;
    change add;
public:
    explicit vertex(const basic& val_,const change& add_)
    {
        val = val_;
        add = add_;
    }
};

template<class basic, class change, class Compos_add, class Consl, class Use>
class IntervalTree{
    vector<vertex<basic, change> > Tree;
    vector<basic> Data;             // I want unlod Data
    const Compos_add compos_add;
    const Consl consl;
    const Use use;
    const basic val_zero;           // I want unload val_zero
    const change add_zero;
    void update(const change add_intr, const int l, const int r, int i, const int l_now, const int r_now)
    {
        int m_now = (l_now + r_now) /2;
        if(l == l_now &&  r == r_now)
        {
            Tree[i].add = compos_add(Tree[i].add, add_intr);
        }
        else if(r <= m_now)
        {
            update(add_intr, l, r, 2*i, l_now, m_now);
        }
        else if(m_now < l)
        {
            update(add_intr, l, r, 2*i + 1, m_now + 1, r_now);
        }
        else
        {
            update(add_intr, l, r, 2*i, l_now, m_now);
            update(add_intr, l, r, 2*i + 1, m_now + 1, r);
        }
    }
    basic learn(const int l, const int r, const int i, const int l_now, const int r_now, change add_res)
    {
        const int m_now = (l_now + r_now) / 2;
        if(l == l_now && r == r_now)
        {
            add_res = compos_add(add_res, Tree[i].add);
            return use(Tree[i].val, add_res);
        }
        else
        {
            add_res = compos_add(add_res, Tree[i].add);
            if(l <= m_now)
            {
                return learn(l, r, 2*i, l_now, m_now, add_res);
            }
            else if(r > m_now)
            {
                return learn(l, r, 2*i+1, m_now+1, r_now, add_res);
            }
            else
            {
                return use(learn(l, r, 2*i, l_now, m_now, add_res), learn(l, r, 2*i+1, m_now+1, r_now, add_res));
            }
        }
    }
public:
    explicit IntervalTree(const int& n, const Use use_, const Consl& consl_, const Compos_add& compos_add_,
                           const change& add_zero_, const basic& val_zero_)
    {
        val_zero = val_zero_;
        add_zero = add_zero_;
        use = use_;
        compos_add = compos_add_;
        consl = consl_;
        Data.resize(basic(add_zero));
        Tree.resize(4*n, vertex<basic, change>(val_zero, add_zero));
    }
    void update(const change add_intr, const int l, const int r)
    {
        update(add_intr, l, r, 1, 1, Data.size());
    }
    basic learn(const int l, const int r)
    {
        return learn(l, r, 1, 1, Data.size(), add_zero);
    }
};

int main()
{

    return 0;
}
