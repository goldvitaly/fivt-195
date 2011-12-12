#ifndef SLOWLYTREE_H_INCLUDED
#define SLOWLYTREE_H_INCLUDED

#include <vector>
#include <algorithm>

class SlowlyTree{
    std::vector<int> Data;
    int sizeTree;
public:
    explicit SlowlyTree(const int sizeTree_)
    {
        sizeTree = sizeTree_;
        Data.resize(sizeTree);
    }
    void update_add(const int mod, const int l, const int r)
    {
        for(int i = l - 1; i < r; i++)
            Data[i] += mod;
    }
    void update_assign(const int mod, const int l, const int r)
    {
        for(int i = l - 1; i < r; i++)
            Data[i] = mod;
    }
    int query_min(const int l, const int r) const
    {
        int ans = Data[l-1];
        for(int i = l; i < r; i++)
            ans = std::min(ans, Data[i]);
        return ans;
    }
    int query_sum(const int l, const int r) const
    {
        int ans = 0;
        for(int i = l - 1; i < r; i++)
            ans += Data[i];
        return ans;
    }
};
#endif // SLOWLYTREE_H_INCLUDED
