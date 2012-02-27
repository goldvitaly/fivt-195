#include <memory.h>
#include <vector>

template<class Iterator>
void RadixSort(Iterator left, Iterator right)
{
    typedef typename std::iterator_traits<Iterator>::value_type T;
    const int BlockNumb = 12;
    std::vector<T> sorted(right-left+1);
    std::vector<int> cnt(((1 << BlockNumb) + 1));
    T index;
    int k, mask, bit = ((8 * sizeof(T)) / BlockNumb - 1) * BlockNumb;

    for (k=0; k<=bit; k+=BlockNumb)
    {
        mask = (1 << (k+BlockNumb)) - (1 << k);
        memset(&cnt[0], 0, sizeof(int) * ((1 << BlockNumb) + 1));
        for (Iterator cur=left; cur!=right; cur++)
        {
            index = ((*cur) & mask) >> k;
            cnt[index]++;
        }
        for (int i=1; i<=(1 << BlockNumb); i++)
            cnt[i] += cnt[i-1];

        for (Iterator i = right-1; i >= left; i--)
        {
            index = ((*i) & mask) >> k;
            cnt[index]--;
            sorted[cnt[index]] = (*i);
        }
        for (Iterator i=left; i!=right; i++)
            *i = sorted[i-left];
    }
}

