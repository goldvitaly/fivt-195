#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <map>
#include <ctime>
#include <cstdlib>

template < typename KeyType, typename ValueType, typename Comp = std::less<KeyType> >
class LeftHeap
{
private:
    Comp comp;
    struct Node
    {
        KeyType key;
        ValueType value;
        Node * leftChild;
        Node * rightChild;
        size_t s;// For any node x in an extended binary tree,  let s(x) be the length of a shortest path from x to an external node in the subtree rooted at x
        Node (const KeyType & k, const ValueType & v):
            key(k), value(v), leftChild(NULL), rightChild(NULL), s(1) {}
        ~Node ()
        {
            delete leftChild;
            delete rightChild;
        }
    };
    size_t sizeHeap;
    Node * rootHeap;
    Node * merge(Node * x, Node * y)
    {
        if (x == NULL)
            return y;
        if (y == NULL)
            return x;
        if (comp(y->key, x->key))
            std::swap(x, y);

        x->rightChild = merge(x->rightChild, y);

        if (x->leftChild == NULL)
        {
            std::swap(x->leftChild, x->rightChild);
			x->s = 1;
        }
        else
        {
            // left child does exist, so compare s-values
            if (x->leftChild->s < x->rightChild->s)
                std::swap(x->leftChild, x->rightChild);
            if (x->rightChild != NULL)
                x->s = x->rightChild->s + 1;
            else
                x->s = 1;
        }
        return x;
    }


public:
    LeftHeap(): rootHeap(NULL) {}
    LeftHeap (const KeyType & k, const ValueType & v):
        rootHeap(k, v), sizeHeap(1) {}
    ~LeftHeap()
    {
        delete rootHeap;
    }
    bool empty() const
    {
        return sizeHeap == 0;
    }
    size_t size() const
    {
        return sizeHeap;
    }

    ValueType top() const
    {
        assert(!empty());
        return rootHeap->value;
    }
    void pop()
    {
        assert(!empty());
        sizeHeap--;
        Node * oldRoot = rootHeap;
        rootHeap = merge(rootHeap->leftChild, rootHeap->rightChild);
        oldRoot->leftChild = NULL;
        oldRoot->rightChild = NULL;
        delete oldRoot;
    }
    void push(const KeyType& k, const ValueType& v)
    {
        sizeHeap++;
        Node * newElem = new Node(k, v);
        rootHeap = merge(rootHeap, newElem);
    }
    void merge(LeftHeap h)
    {
        sizeHeap += h.size();
        rootHeap = merge(rootHeap, h->rootHeap);
    }
};

void test()
{
    std::srand( time(NULL) );
    int n = 1000000;
    LeftHeap<int, int> root;
    std::vector< std::pair<int, int> > a;
    for (int i = 0; i < n; i++)
        a.push_back(std::make_pair(i, i * i));
    std::random_shuffle(a.begin(), a.end());
    std::map<int, int> M;
    int k = 0, b = 0;
    bool ok = true;
    while (k < n)
    {
        if (std::rand() & 1 || b == 0)
        {
            M[a[k].first] = a[k].second;
            root.push(a[k].first, a[k].second);
            k++;
            b++;
        }
        else
        {
            int v = root.top();
            root.pop();
            if (v != (*M.begin()).second)
            {
                ok = false;
                break;
            }
            M.erase(M.begin());
            b--;
        }
    }

    if (ok)
        std::cout << "OK";
    else
        std::cout << "BAD";
}
int main()
{
    test();
    return 0;
}
