#include <iostream>
#include <vector>
#include <algorithm>
#include "binomial_heap.h"



int main()
{
    std::vector <binomial_heap <int> > H;
    H.resize(rand() % 300 + 10);
    for(int i = 0; i < H.size(); i++)
    {
        int j = rand() % 1000 + 10;
        while(j)
        {
            H[i].insert(rand() % 10000);
            j--;
        }
    }
    for(int i = 1; i < H.size(); i++)
    {
        int minimum = std::min(H[0].get_min(), H[i].get_min());
        H[0].merge_and_del(&H[i]);
        if(!H[0].check_sizes())   std::cout << "OMFG\n";
        if(H[0].get_min() != minimum)   std::cout << "OMFG\n";
    }
    return 0;
}
