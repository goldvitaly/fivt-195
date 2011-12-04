#ifndef INTERVALTREE_H_INCLUDED
#define INTERVALTREE_H_INCLUDED

#include <vector>

using namespace std;

template<class Element, class Modif>
class vertex{
    Element val;
    Modif add;
public:
    explicit vertex(const Element& val_, const Modif& add_)
    {
        val = val_;
        add = add_;
    }
    void change_add(const Modif& new_add_)
    {
        add = new_add_;
    }
    void change_val(const Element& new_val_)
    {
        val = new_val_;
    }
    Modif get_add() const{
        return add;
    }
    Element get_val() const{
        return val;
    }
};

template<class Element, class Modif, class Use, class Conslid, class ComposAdd>
class IntervalTree{
    vector<vertex<Element, Modif> > Tree;
    ComposAdd composAdd;
    Conslid conslid;
    Use use;
    int treeSize;
    Modif addZero;
    void Constructor(const int n, const Use& use_, const Conslid& conslid_, const ComposAdd& composAdd_,
                           const Modif& addZero_)
    {
        treeSize = n;
        addZero = addZero_;
        use = use_;
        composAdd = composAdd_;
        conslid = conslid_;
    }
    void make_tree(const int i, const int l, const int r, const vector<Element>& Data)
    {
        if(l == r)
        {
            Tree[i].change_val(Data[l-1]);
        }
        else
        {
            const int m = (l + r) / 2;
            make_tree(2*i, l, m, Data);
            make_tree(2*i+1, m+1, r, Data);
            Tree[i].change_val(conslid(Tree[2*i].get_val(), Tree[2*i+1].get_val()));
        }
    }
    void update(const Modif& addIntroduce, const int l, const int r, int i, const int l_now, const int r_now)
    {
        if(l == l_now &&  r == r_now)
        {
            Tree[i].change_add(composAdd(Tree[i].get_add(), addIntroduce));
        }
        else
        {
            const int m_now = (l_now + r_now) / 2;
            if(r <= m_now)
            {
                update(addIntroduce, l, r, 2*i, l_now, m_now);
            }
            else if(m_now < l)
            {
                update(addIntroduce, l, r, 2*i + 1, m_now + 1, r_now);
            }
            else
            {
                update(addIntroduce, l, m_now, 2*i, l_now, m_now);
                update(addIntroduce, m_now+1, r, 2*i + 1, m_now + 1, r_now);
            }
        }
    }
    Element query(const int l, const int r, const int i, const int l_now, const int r_now, Modif addRes)
    {
        if(l == l_now && r == r_now)
        {
            addRes = composAdd(addRes, Tree[i].get_add());
            return use(Tree[i].get_val(), addRes);
        }
        else
        {
            const int m_now = (l_now + r_now) / 2;
            addRes = composAdd(addRes, Tree[i].get_add());
            if(r <= m_now)
            {
                return query(l, r, 2*i, l_now, m_now, addRes);
            }
            else if(l > m_now)
            {
                return query(l, r, 2*i+1, m_now+1, r_now, addRes);
            }
            else
            {
                return conslid(query(l, m_now, 2*i, l_now, m_now, addRes),
                                query(m_now+1, r, 2*i+1, m_now+1, r_now, addRes));
            }
        }
    }
public:
    IntervalTree(const int n, const Use& use_, const Conslid& conslid_, const ComposAdd& composAdd_,
                           const Modif& addZero_, const Element& valZero_)
    {
        Constructor(n, use_, conslid_, composAdd_, addZero_);
        Tree.resize(4*n, vertex<Element, Modif>(valZero_, addZero));
    }

    IntervalTree(const int n, const Use& use_, const Conslid& conslid_, const ComposAdd& composAdd_,
                           const Modif& addZero_, const vector<Element>& Data)
    {
        Constructor(n, use_, conslid_, composAdd_, addZero_);
        Tree.resize(4*n, vertex<Element, Modif>(Element(), addZero));
        make_tree(1, 1, treeSize, Data);
    }
    void update(const Modif& addIntroduce, const int l, const int r)
    {
        update(addIntroduce, l, r, 1, 1, treeSize);
    }
    Element query(const int l, const int r)
    {
        return query(l, r, 1, 1, treeSize, addZero);
    }
    size_t size() const {return treeSize;}
};
#endif // INTERVALTREE_H_INCLUDED
