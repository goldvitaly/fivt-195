#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "interval_tree.h"





class sum_int
{
    public:
    int merge(int x, int y)
    {
        return (x + y);
    }
};

class modify_int
{
    public:
    int modify(int el, int mod, const size_t &range_size)
    {
        return (el + (mod * range_size));
    }
};

void generate_interval(int *left, int *right, size_t max)
{
    *right = rand() % max + 1;
    *left = rand() % *right + 1;
}


int main()
{
    srand(42);
    size_t number_of_elements, number_of_tests;
    std::vector <int> A, S; //S - vector of partitial sums
    std::cout << "Welcome to interval_tree test program!\nTest works in O(N*K) time where\nN - number of elements\nK - number of tests\nPlease, insert N and K:\n";
    std::cin >> number_of_elements >> number_of_tests;
    A.resize(number_of_elements, 0);
    S.resize(number_of_elements, 0);
    int left, right, delta;
    interval_tree<int, int, sum_int, modify_int, sum_int> tree1 = interval_tree<int, int, sum_int, modify_int, sum_int>(A.begin(), A.end(), 0);
    bool fail = false;
    for(int i = 0; i < number_of_tests; i++)
    {
        generate_interval(&left, &right, A.size());
        delta = rand() % 1000000;
        if((rand() & 1))    delta = -delta;
        tree1.mod_range(left, right, delta);
        for(int j = left - 1; j < right; j++)
        {
            A[j] += delta;
            if(j)
                S[j] = S[j - 1];
            else
                S[j] = 0;
            S[j] += A[j];
        }
        for(int j = right; j < S.size(); j++)
            S[j] = S[j - 1] + A[j];
        generate_interval(&left, &right, A.size());
        delta = S[right - 1];              //we calculate partitial sum and push it in delta
        if(left - 1) delta -= S[left - 2];
        if(tree1.get_from_range(left, right) != delta)      //compare delta with interval_tree function
            fail = true;
    }
    if(!fail)   std::cout << "Passed system test\n";
    else        std::cout << "Failed system test\n";
    return 0;
}
