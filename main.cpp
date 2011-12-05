#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "interval_tree.h"





class merge_int
{
    public:
    int merge(const int &x, const int &y)
    {
        return (x + y);
    }
};

class modify_int
{
    public:
    int modify(const int &el, const int &mod, const size_t &range_size)
    {
        return (el + (mod * range_size));
    }
};




int main()
{
    srand(42);
    std::vector <int> A;
    A.resize(5, 0);
    interval_tree<int, int, merge_int, modify_int, merge_int> tree1 = interval_tree<int, int, merge_int, modify_int, merge_int>(A, 0);
    tree1.mod_range(2, 3, 4);
    tree1.mod_range(3, 5, 3);
    std::cout  << tree1.get_from_range(3, 5) << ' ' << tree1.get_from_range(2, 4) << ' ' << tree1.get_from_range(2, 5) << std::endl;
    return 0;
}
