#ifndef RADIX_SORT_H_INCLUDED
#define RADIX_SORT_H_INCLUDED

#include <algorithm>
#include <vector>
#include <numeric>


template <class Type, class Iter, class BlokExtract>
void radix_sort(Iter l, Iter r, BlokExtract Ext)
{
    std::vector<int> ArrayKey(Ext.max_key() + 1);
    std::vector<Type> ans(r-l);

    Iter L[2], R[2];
    L[0] = l;
    R[0] = r;
    L[1] = ans.begin();
    R[1] = ans.end();
    size_t num_bl = 0;
    for(size_t i = 0; i < Ext.size(); i += Ext.blok(), num_bl++)
    {
        std::fill(ArrayKey.begin(), ArrayKey.end(), 0);
        Iter start = L[num_bl%2], end = R[num_bl%2];
        for(Iter j = start; j < end; j++)
        {
            int blok = Ext.extract(*j, i);
            ArrayKey[blok + 1]++;
        }
        std::partial_sum(ArrayKey.begin(), ArrayKey.end(), ArrayKey.begin());
        Iter outStart = L[(num_bl + 1)%2];
        for(Iter j = start; j < end; j++)
        {
            int blok = Ext.extract(*j, i);
            *(outStart + ArrayKey[blok]++) = *j;
        }
    }
    if(num_bl%2 == 1)
    {
        std::copy(L[1], R[1], L[0]);
    }
}

#endif // RADIX_SORT_H_INCLUDED
