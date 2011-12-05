#ifndef INTERVALTREE_H_INCLUDED
#define INTERVALTREE_H_INCLUDED

#include <vector>
#include <iostream>
#include <cstdlib>

template<class Element, class Modif>
class vertex{
    Element val;
    Modif add;
    bool flagMod;
public:
    explicit vertex(const Element& val_, const Modif& add_)
    {
        val = val_;
        add = add_;
        flagMod = false;
    }
    void change_add(const Modif& new_add_)
    {
        add = new_add_;
    }
    void change_val(const Element& new_val_)
    {
        val = new_val_;
    }
    void change_flag(const bool flagMod_)
    {
        flagMod = flagMod_;
    }
    Modif get_add() const{
        return add;
    }
    Element get_val() const{
        return val;
    }
    bool get_flag() const{
        return flagMod;
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
    std::vector<vertex<Element, Modif> > Tree;
    ComposAdd composAdd;
    Conslid conslid;
    Use use;
    Modif addZero;
    int treeSize;
    void push_modif(const int v)
    {
        Tree[v].change_val(use(Tree[v].get_val(), Tree[v].get_add()));
        if(2*v < Tree.size())
        {
            Tree[2*v].change_add(composAdd(Tree[2*v].get_add(), Tree[v].get_add()));
            Tree[2*v].change_flag(true);
        }
        if(2*v + 1 < Tree.size())
        {
            Tree[2*v+1].change_add(composAdd(Tree[2*v+1].get_add(), Tree[v].get_add()));
            Tree[2*v+1].change_flag(true);
        }
        Tree[v].change_add(addZero);
        Tree[v].change_flag(false);
    }
    void make_tree(const int v, const segment& viewInterval, const std::vector<Element>& Data)
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
        if(Tree[v].get_flag())
        {
            push_modif(v);
        }
        if(modInterval == viewInterval)
        {
            Tree[v].change_add(composAdd(Tree[v].get_add(), addIntroduce));
            Tree[v].change_flag(true);
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
            Element leftChild = use(Tree[2*v].get_val(), Tree[2*v].get_add());
            Element rightChild = use(Tree[2*v + 1].get_val(), Tree[2*v + 1].get_add());
            Tree[v].change_val(conslid(leftChild, rightChild));
        }
    }
    Element query(const segment& modInterval, const int v, const segment& viewInterval, Modif addRes)
    {
        if(Tree[v].get_flag())
        {
            push_modif(v);
        }
        if(modInterval == viewInterval)
        {
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
                Element leftChild = query(modInterval.left_part(viewInterval.middle()), 2*v,
                                            viewInterval.left_part(), addRes);
                Element rightChild = query(modInterval.right_part(viewInterval.middle()), 2*v+1,
                                             viewInterval.right_part(), addRes);
                return conslid(leftChild, rightChild);
            }
        }
    }
public:
    IntervalTree(const int n, const Modif& addZero_, const Element& valZero_)
    {
        treeSize = n;
        addZero = addZero_;
        Tree.resize(4*n, vertex<Element, Modif>(valZero_, addZero));
    }

    IntervalTree(const int n, const Modif& addZero_, const std::vector<Element>& Data)
    {
        treeSize = n;
        addZero = addZero_;
        Tree.resize(4*n, vertex<Element, Modif>(Element(), addZero));
        make_tree(1, segment(1, treeSize), Data);
    }
    void update(const Modif& addIntroduce, const int l, const int r)
    {
        if(l > r)
        {
            std::cout << "Error l > r" << std::endl;
            exit(1);
        }
        update(addIntroduce, segment(l, r), 1, segment(1, treeSize));
    }
    Element query(const int l, const int r)
    {
        if(l > r)
        {
            std::cout << "Error l > r" << std::endl;
            exit(1);
        }
        return query(segment(l, r), 1, segment(1, treeSize), addZero);
    }
    size_t size() const {return treeSize;}
};
#endif // INTERVALTREE_H_INCLUDED
