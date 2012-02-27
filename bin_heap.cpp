#include "bheap.h"
#include <algorithm>
#include <iostream>

using namespace std;

int main()
{

    BHeap<int> a, b;
    srand(532454);
    cout<<"size = 10, a: ";
    for (int i=0; i<10; i++)
    {
        int x = -rand();
        cout<< x<<' ';
        a.Push(x);
    }
    cout<< "\n\nsize = 9, b: ";
    for (int i=0; i<9; i++)
    {
        int x = rand();
        cout<< x<<' ';
        b.Push(x);
    }
    cout<< "\n\na.Top: ";
    cout<< a.Top()<<"\nb.Top: "<<b.Top()<<"\n\nNEW a.Top: ";
    a.PopTop();
    b.PopTop();
    cout<< a.Top()<<"\nNEW b.Top: "<<b.Top()<<"\n\nsize a: ";
    cout<< a.Size()<<"\nsize b: "<<b.Size();
    return 0;
}
