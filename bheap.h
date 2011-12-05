#include <algorithm>
#include <iostream>
#include <memory.h>
#include <cstdlib>
#include <time.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <set>
#include <map>

#define mp make_pair
#define f first
#define s second
#define pb push_back

using namespace std;

typedef long long lglg;
typedef double db;


template <class T>
class BHeap
{
public:
    void Clear()
    {
        size = 0;
        CClear(head);
    }
    int Size()
    {
        return size;
    }

    BHeap()
    {
        head = NULL;
        size = 0;
    }
    void Push(const T &key)
    {
        BHeap<T> temp;
        Node *node = new Node;
        node->key = key;
        node->par = node->child = node->sibling = NULL;
        node->degree = 0;
        temp.head = node;
        temp.size = 1;
        size++;
        BHeapUnion(node);
    }
    const T& Top()
    {
        if (size == 0)
            throw 0;
        Node *x = head;
        T mn = x->key;

        while (x != NULL)
        {
            if (x->key < mn)
            {
                mn = x->key;
            }
            x = x->sibling;
        }
        return mn;
    }
    void Add(BHeap<T> &x)
    {
        size += x.size;
        BHeapUnion(x.head);
    }
    void PopTop()
    {
        if (size == 0)
            throw 1;
        Node *x = head, *prev, *mn = head;

        while (x != NULL)
        {
            if (x->key < mn->key)
            {
                mn = x;
            }
            prev = x;
            x = x->sibling;
        }

        prev->sibling = mn->sibling;
        vector<Node*> temp;

        for (Node *i = mn->child; i != NULL; i = i->sibling)
        {
            temp.pb(i);
        }

        for (int i = 0; i<temp.size(); i++)
        {
            temp[i]->sibling = temp[i]->par = NULL;
            BHeapUnion(temp[i]);
        }
        delete mn;
        size--;
    }

    ~BHeap()
    {
        return;
        if (head != NULL)
        {
            CClear(head);
        }

    }

private:
    struct Node
    {
        T key;
        int degree;
        Node *par, *child, *sibling;
    };
    Node *head;
    int size;
    void CClear(Node *&temp)
    {
        if (temp == NULL)
            return;
        for (Node *i = temp; i != NULL; i = i->sibling)
            for (Node *&j = i->child; j != NULL; j = j->sibling)
                CClear(j);
         delete temp;

    }
    void BHeapLink(Node *a, Node *b)
    {
        b->par = a;
        b->sibling = a->child;
        a->child = b;
        a->degree++;
    }
    Node* BHeapMerge(Node *a, Node *b)
    {
        Node *temp = NULL, *ans;
        while (a != NULL || b != NULL)
        {
            //if (a != NULL)
                //  cout<< a<<' ';

           // if (a != NULL)
            //    cout<< (a -> degree);

            if (b == NULL || (a != NULL && a->degree <= b->degree))
            {
                if (temp == NULL)
                {
                    ans = temp = a;
                    a = a->sibling;
                }
                else
                {
                    temp->sibling = a;
                    temp = a;
                    a = a->sibling;
                }
            }
            else
            {
                if (temp == NULL)
                {
                    ans = temp = b;
                    b = b->sibling;
                }
                else
                {
                    temp->sibling = b;
                    temp = b;
                    b = b->sibling;
                }
            }
        }
        return ans;
    }
    void BHeapUnion(Node *temp_head)
    {
        head = BHeapMerge(head, temp_head);
        if (head == NULL)
            return;
        Node *x = head;
        Node *next = x->sibling, *prev = NULL;
        while (next != NULL)
        {
            if ((x->degree != next->degree) ||
            (next->sibling != NULL && next->sibling->degree == x->degree))
            {
                prev = x;
                x = next;
            }
            else
            {
                if (x->key <= next->key)
                {
                    x->sibling = next->sibling;
                    BHeapLink(x, next);
                }
                else
                {
                    if (prev == NULL)
                        head = next;
                    else
                        prev->sibling = next;
                    BHeapLink(next, x);
                    x = next;
                }
            }
            next = x->sibling;
        }
    }
};
