#ifndef RADIX_SORT_H_INCLUDED
#define RADIX_SORT_H_INCLUDED

#include <algorithm>
#include <vector>
#include <numeric>


template <class Type, class Iter, class BlokExtract>
void radix_sort(Iter l, Iter r, BlokExtract ext)
{
    std::vector<int> arrayKey(ext.max_key() + 1);
    std::vector<Type> ans(r-l);

    Iter L[2], R[2];
    L[0] = l;
    R[0] = r;
    L[1] = ans.begin();
    R[1] = ans.end();
    size_t numBlok = 0;
    for(size_t i = 0; i < ext.size(); i += ext.blok(), numBlok++)
    {
        std::fill(arrayKey.begin(), arrayKey.end(), 0);
        Iter start = L[numBlok%2], end = R[numBlok%2];
        for(Iter j = start; j < end; j++)
        {
            int blok = ext.extract(*j, i);
            arrayKey[blok + 1]++;
        }
        std::partial_sum(arrayKey.begin(), arrayKey.end(), arrayKey.begin());
        Iter outStart = L[(numBlok + 1)%2];
        for(Iter j = start; j < end; j++)
        {
            int blok = ext.extract(*j, i);
            *(outStart + arrayKey[blok]++) = *j;
        }
    }
    if(numBlok%2 == 1)
    {
        std::copy(L[1], R[1], L[0]);
    }
}

#endif // RADIX_SORT_H_INCLUDED
