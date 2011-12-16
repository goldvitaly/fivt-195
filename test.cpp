#include <iostream>
#include <cstdlib>
#include <ctime>
#include "BinomialHeap.h"

using namespace std;

void test(const int& n)
{
    typedef int T;
    srand(0);
    vector<T> V(n);

    for(int i = 0; i < n; i++)
        V[i] = rand();

    BinHeap<T>* heap1 = new BinHeap<T>;
    BinHeap<T>* heap2 = new BinHeap<T>;

    const int middle = rand() % n;
    for(int i = 0; i < middle; i++)
        BinHeap<T>::push(heap1, V[i]);
    for(int i = middle; i < n; i++)
        BinHeap<T>::push(heap2, V[i]);

    assert(heap1->size() == middle);
    assert(heap2->size() == n - middle);

    BinHeap<T>* heap3 = BinHeap<T> :: merge(heap1, heap2);
    assert(heap1 == NULL);
    assert(heap2 == NULL);
    sort(V.begin(), V.end());
    for(int i = 0; i < n; i++)
    {
        if(V[i] != heap3->min())
        {
            cerr << "Wrong work" << endl;
            exit(1);
        }
        BinHeap<T>::pop(heap3);
    }
    assert(heap3->empty());
    delete heap1;
    delete heap2;
    delete heap3;
}

void test_merge()
{
    typedef int T;
    srand(0);
    BinHeap<T>* heap1 = new BinHeap<T>;
    BinHeap<T>* heap2 = new BinHeap<T>;
    for(int i = 0; i < 100000; i++)
    {
        BinHeap<T>::push(heap1, rand());
        BinHeap<T>::push(heap2, rand());
    }
    BinHeap<T>* heap3 = BinHeap<T> :: merge(heap1, heap2);
    delete heap1;
    delete heap2;
    delete heap3;
}

int main()
{
    for(int n = 10; n <= 100000; n *= 10)
        for(int trial = 0; trial < 5; trial++)
            test(n);
    for(int trial = 0; trial < 4; trial++)
        test_merge();
    cerr << "Ok" << endl;
    return 0;
}
