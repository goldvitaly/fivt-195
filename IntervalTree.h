#ifndef INTERVALTREE_H_INCLUDED
#define INTERVALTREE_H_INCLUDED

#include <vector>

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
    void new_add(const change& new_add_)
    {
        add = new_add_;
    }
    void new_val(const basic& new_val_)
    {
        val = new_val_;
    }
    change see_add() const{
        return add;
    }
    basic see_val() const{
        return val;
    }
};

template<class basic, class change, class Use, class Consl, class Compos_add>
class IntervalTree{
    vector<vertex<basic, change> > Tree;
    Compos_add compos_add;
    Consl consl;
    Use use;
    int tree_size;
    change add_zero;
    void IntervalTree2(const int& n, const Use& use_, const Consl& consl_, const Compos_add& compos_add_,
                           const change& add_zero_)
    {
        tree_size = n;
        add_zero = add_zero_;
        use = use_;
        compos_add = compos_add_;
        consl = consl_;
    }
    void make_tree(const int i, const int l, const int r, const vector<basic>& Data)
    {
        if(l == r)
        {
            Tree[i].new_val(Data[l-1]);
        }
        else
        {
            const int m = (l + r) / 2;
            make_tree(2*i, l, m, Data);
            make_tree(2*i+1, m+1, r, Data);
            Tree[i].new_val(consl(Tree[2*i].see_val(), Tree[2*i+1].see_val()));
        }
    }
    void update(const change add_intr, const int l, const int r, int i, const int l_now, const int r_now)
    {
        if(l == l_now &&  r == r_now)
        {
            Tree[i].new_add(compos_add(Tree[i].see_add(), add_intr));
        }
        else
        {
            const int m_now = (l_now + r_now) / 2;
            if(r <= m_now)
            {
                update(add_intr, l, r, 2*i, l_now, m_now);
            }
            else if(m_now < l)
            {
                update(add_intr, l, r, 2*i + 1, m_now + 1, r_now);
            }
            else
            {
                update(add_intr, l, m_now, 2*i, l_now, m_now);
                update(add_intr, m_now+1, r, 2*i + 1, m_now + 1, r_now);
            }
        }
    }
    basic learn(const int l, const int r, const int i, const int l_now, const int r_now, change add_res)
    {
        if(l == l_now && r == r_now)
        {
            add_res = compos_add(add_res, Tree[i].see_add());
            return use(Tree[i].see_val(), add_res);
        }
        else
        {
            const int m_now = (l_now + r_now) / 2;
            add_res = compos_add(add_res, Tree[i].see_add());
            if(r <= m_now)
            {
                return learn(l, r, 2*i, l_now, m_now, add_res);
            }
            else if(l > m_now)
            {
                return learn(l, r, 2*i+1, m_now+1, r_now, add_res);
            }
            else
            {
                return consl(learn(l, m_now, 2*i, l_now, m_now, add_res), learn(m_now+1, r, 2*i+1, m_now+1, r_now, add_res));
            }
        }
    }
public:
    explicit IntervalTree(const int& n, const Use& use_, const Consl& consl_, const Compos_add& compos_add_,
                           const change& add_zero_, const basic& val_zero_)
    {
        IntervalTree2(n, use_, consl_, compos_add_, add_zero_);
        Tree.resize(4*n, vertex<basic, change>(val_zero_, add_zero));
    }

    explicit IntervalTree(const int& n, const Use& use_, const Consl& consl_, const Compos_add& compos_add_,
                           const change& add_zero_, const vector<basic>& Data)
    {
        IntervalTree2(n, use_, consl_, compos_add_, add_zero_);
        Tree.resize(4*n, vertex<basic, change>(basic(), add_zero));
        make_tree(1, 1, tree_size, Data);
    }
    void update(const change add_intr, const int l, const int r)
    {
        update(add_intr, l, r, 1, 1, tree_size);
    }
    basic learn(const int l, const int r)
    {
        return learn(l, r, 1, 1, tree_size, add_zero);
    }
    int size() const {return tree_size;}
};
#endif // INTERVALTREE_H_INCLUDED
