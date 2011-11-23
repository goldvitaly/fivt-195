#include <iostream>
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

    const int medium = rand() % n;
    for(int i = 0; i < medium; i++)
        heap1 = heap1->push(V[i]);
    for(int i = medium; i < n; i++)
        heap2 = heap2->push(V[i]);

    assert(heap1->size() == medium);
    assert(heap2->size() == n - medium);

    BinHeap<T>* heap3 = BinHeap<T> :: merge(heap1, heap2);

    sort(V.begin(), V.end());
    for(int i = 0; i < n; i++)
    {
        if(V[i] != heap3->min())
        {
            cout << "Not" << endl;
            return;
        }
        heap3 = heap3->pop();
    }
    assert(heap3->empty());
    delete heap3;
}

int main()
{
    for(int n = 10; n <= 10000; n *= 10)
        for(int trial = 0; trial < 5; trial++)
            test(n);
    return 0;
}
