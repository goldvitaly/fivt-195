#ifndef BINOMIALHEAP_H_INCLUDED
#define BINOMIALHEAP_H_INCLUDED

#include <vector>
#include <cstdlib>

template<class T>
class BinTree{
public:
    explicit BinTree(const T& key = 0)
    {
        key_ = key;
    }
    ~BinTree()
    {
        for(size_t i = 0; i < children.size(); i++)
        {
            delete children[i];
        }
    }
    const T& min() const
    {
        return key_;
    }
    static BinTree<T>* merge(BinTree<T>*& tree1, BinTree<T>*& tree2)
    {
        assert(tree1->num_children() == tree2->num_children());
        if(tree1->min() > tree2->min())
        {
            swap(tree1, tree2);
        }
        tree1->children.push_back(tree2);
        BinTree<T>* ans = new BinTree<T>;
        ans = tree1;
        tree1 = new BinTree<T>;
        tree2 = new BinTree<T>;
        return ans;
    }
    size_t num_children() const
    {
        return children.size();
    }
    size_t size() const
    {
        return num_children();
    }
    const std::vector<BinTree<T>*>& get_children()
    {
        return children;
    }
    void clear_children()
    {
        children.clear();
    }
private:
    T key_;
    std::vector<BinTree<T>*> children;
};

template<class T>
class BinHeap{
public:
    BinHeap(){};
    explicit BinHeap(const T& key)
    {
        link.push_back(new BinTree<T>(key));
    }
    explicit BinHeap(const BinTree<T>*& tree)
    {
        link.resize(tree->num_children());
        link.push_back(tree);
    }
    explicit BinHeap(const std::vector<BinTree<T>*>& link_)
    {
        link = link_;
    }
    ~BinHeap()
    {
        for(size_t i = 0; i < link.size(); i++)
        {
            delete link[i];
        }
    }
    T min() const
    {
        return link[min_id()]->min();
    }
    static void pop(BinHeap<T>*& heap)
    {
        int id = heap->min_id();
        BinTree<T>* remove_tree = heap->link[id];
        std::vector<BinTree<T>*> children_remove_link = remove_tree->get_children();
        remove_tree->clear_children();
        delete remove_tree;
        heap->link[id] = NULL;
        BinHeap<T>* res = new BinHeap<T>(children_remove_link);
        heap = merge(heap, res);
    }
    static BinHeap<T>* merge(BinHeap<T>*& heap1, BinHeap<T>*& heap2)
    {
        BinHeap<T>* ans = new BinHeap<T>;
        BinTree<T>* flag = NULL;
        for(size_t i = 0; i < heap1->link.size() || i < heap2->link.size() || flag != NULL; i++)
        {
            if(ans->link.size() <= i)
            {
                ans->link.resize(i + 1);
            }
            std::vector<BinTree<T>*> trees;
            trees.reserve(3);
            if(heap1->exist_tree(i))
                trees.push_back(heap1->link[i]);
            if(heap2->exist_tree(i))
                trees.push_back(heap2->link[i]);
            if(flag != NULL)
                trees.push_back(flag);

            for(int j = 0; j < trees.size(); j++)
                assert(trees[j]->size() == i);

            if(trees.size() == 1)
            {
                ans->link[i] = trees[0];
                flag = NULL;
            }
            else if(trees.size() >= 2)
            {
                flag = BinTree<T>::merge(trees[0], trees[1]);
                if(trees.size() == 3)
                {
                    ans->link[i] = trees[2];
                }
            }
        }
        ans->del_last_zero();
        heap1 = new BinHeap<T>;
        heap2 = new BinHeap<T>;
        return ans;
    }
    static void push(BinHeap<T>*& heap, const T& key)
    {
        BinHeap<T>* newHeap = new BinHeap<T>(key);
        heap = merge(heap, newHeap);
    }
    bool empty() const
    {
        return link.empty();
    }
    size_t size() const                                      // O(link)
    {
        size_t ans = 0;
        for(size_t i = 0; i < link.size(); i++)
        {
            if(link[i] != NULL)
                ans += 1 << i;
        }
        return ans;
    }
private:
    std::vector<BinTree<T>*> link;
    int min_id() const
    {
        int id = 0;
        bool found = false;
        for(size_t it = 0; it < link.size(); it++)
        {
            if((link[it] != NULL) && (!found || link[it]->min() < link[id]->min()))
            {
                id = it;
                found = true;
            }
        }
        if(!found)
        {
            std::cerr << "Empty heap" << std::endl;
            exit(1);
        }
        return id;
    }
    void del_last_zero()
    {
        int current_size = link.size();
        while(current_size > 0 && link[current_size - 1] == NULL)
        {
            current_size--;
        }
        link.resize(current_size);
    }
    bool exist_tree(int ind) const
    {
        return (ind < link.size() && link[ind] != NULL);
    }
};

#endif // BINOMIALHEAP_H_INCLUDED
