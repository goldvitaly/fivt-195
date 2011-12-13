#include <iostream>
#include <cstdlib>
#include <ctime>
#include "BinomialHeap.h"

using namespace std;

void test(const int& n)
{
    typedef int T;
    srand(time(NULL));
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

    assert(heap1->empty());
    assert(heap2->empty());

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

int main()
{
    for(int n = 10; n <= 10000; n *= 10)
        for(int trial = 0; trial < 5; trial++)
            test(n);
    cerr << "Ok" << endl;
    return 0;
}
