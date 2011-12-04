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

struct segment{
    int l, r;
    segment(int l_, int r_)
    {
        l = l_;
        r = r_;
    }
    bool operator==(segment s) const
    {
        return (s.l == l && s.r == r);
    }
    int middle() const
    {
        return (l + r)/2;
    }
    segment left_part(int section) const
    {
        return segment(l, section);
    }
    segment left_part() const
    {
        return segment(l, middle());
    }
    segment right_part(int section) const
    {
        return segment(section + 1, r);
    }
    segment right_part() const
    {
        return segment(middle() + 1, r);
    }
    size_t size() const{
        return r - l + 1;
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
    void make_tree(const int v, const segment& viewInterval, const vector<Element>& Data)
    {
        if(viewInterval.size() == 1)
        {
            Tree[v].change_val(Data[viewInterval.l-1]);
        }
        else
        {
            make_tree(2*v, viewInterval.left_part(), Data);
            make_tree(2*v+1, viewInterval.right_part(), Data);
            Tree[v].change_val(conslid(Tree[2*v].get_val(), Tree[2*v+1].get_val()));
        }
    }
    void update(const Modif& addIntroduce, const segment& modInterval, const int v, const segment& viewInterval)
    {
        if(modInterval == viewInterval)
        {
            Tree[v].change_add(composAdd(Tree[v].get_add(), addIntroduce));
        }
        else
        {
            if(modInterval.r <= viewInterval.middle())
            {
                update(addIntroduce, modInterval, 2*v, viewInterval.left_part());
            }
            else if(modInterval.l > viewInterval.middle())
            {
                update(addIntroduce, modInterval, 2*v+1, viewInterval.right_part());
            }
            else
            {
                update(addIntroduce, modInterval.left_part(viewInterval.middle()), 2*v,
                        viewInterval.left_part());
                update(addIntroduce, modInterval.right_part(viewInterval.middle()), 2*v+1,
                        viewInterval.right_part());
            }
        }
    }
    Element query(const segment& modInterval, const int v, const segment& viewInterval, Modif addRes)
    {
        if(modInterval == viewInterval)
        {
            addRes = composAdd(addRes, Tree[v].get_add());
            return use(Tree[v].get_val(), addRes);
        }
        else
        {
            addRes = composAdd(addRes, Tree[v].get_add());
            if(modInterval.r <= viewInterval.middle())
            {
                return query(modInterval, 2*v, viewInterval.left_part(), addRes);
            }
            else if(modInterval.l > viewInterval.middle())
            {
                return query(modInterval, 2*v+1, viewInterval.right_part(), addRes);
            }
            else
            {
                Element left_child = query(modInterval.left_part(viewInterval.middle()), 2*v,
                                            viewInterval.left_part(), addRes);
                Element right_child = query(modInterval.right_part(viewInterval.middle()), 2*v+1,
                                             viewInterval.right_part(), addRes);
                return conslid(left_child, right_child);
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
        make_tree(1, segment(1, treeSize), Data);
    }
    void update(const Modif& addIntroduce, const int l, const int r)
    {
        update(addIntroduce, segment(l, r), 1, segment(1, treeSize));
    }
    Element query(const int l, const int r)
    {
        return query(segment(l, r), 1, segment(1, treeSize), addZero);
    }
    size_t size() const {return treeSize;}
};
#endif // INTERVALTREE_H_INCLUDED
