#include <iostream>
#include <cstdlib>
#include <ctime>
#include "BinomialHeap.h"

using namespace std;

template<class T, class RandConstr>
void test(const int& n)
{
    srand(0);
    vector<T> V(n);

    for(int i = 0; i < n; i++)
        V[i] = RandConstr().gen();
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

class RandConstrInt{
public:
    int gen() const
    {
        return rand();
    }
};

class RandConstrPair{
public:
    pair<int, int> gen() const
    {
        return pair<int, int>(rand(), rand());
    }
};

void test_memory()
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
    assert(heap1 == NULL);
    assert(heap2 == NULL);
    delete heap3;
}

int main()
{
    for(int n = 10; n <= 10000; n *= 10)
        for(int trial = 0; trial < 5; trial++)
        {
            test<int, RandConstrInt>(n);
            test<pair<int, int>, RandConstrPair>(n);
        }

    for(int trial = 0; trial < 4; trial++)
        test_memory();

    cerr << "Ok" << endl;
    return 0;
}
