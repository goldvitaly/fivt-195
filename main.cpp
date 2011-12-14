#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "interval_tree.h"

std::pair<int, int> generate_interval(size_t max)
{
    std::pair<int, int> res;
    res.second = rand() % max + 1;
    res.first = rand() % res.second + 1;
    return res;
}


int main()
{
    srand(time(NULL));
    size_t number_of_elements, number_of_tests;
    std::vector <int> A, S; //S - vector of partitial sums
    std::cout << "Welcome to interval_tree test program!\nTest works in O(N*K) time where\nN - number of elements\nK - number of tests\nPlease, insert N and K:\n";
    std::cin >> number_of_elements >> number_of_tests;
    for(int i = 0; i < number_of_elements; i++)
    {
        int t = 0;//rand() % 1000000;
        A.push_back(t);
        S.push_back(t);
    }
    int delta, left, right, true_sum, true_min;
    std::pair<int, int> seg;
    RMQ<int> min_tree = RMQ<int>(A.begin(), A.end());
    RSQ<int> sum_tree = RSQ<int>(A.begin(), A.end());
    bool fail = false, mod;
    for(int i = 0; i < number_of_tests; i++)
    {
        seg = generate_interval(A.size());
        left = seg.first;
        right = seg.second;
        delta = rand() % 1000000;
        if((rand() & 1))    delta = -delta;
        mod = ((rand() & 1));
        if(mod)
        {
            min_tree.mod_range(left, right, delta);
            sum_tree.mod_range(left, right, delta);
            for(int j = left - 1; j <right; j++)    A[j] += delta;
        }
        else
        {
            min_tree.change_range(left, right, delta);
            sum_tree.change_range(left, right, delta);
            for(int j = left - 1; j <right; j++)    A[j] = delta;
        }
        seg = generate_interval(A.size());
        left = seg.first;
        right = seg.second;
        true_min = std::numeric_limits<int>::max();
        true_sum = 0;
        for(int j = left - 1; j < right; j++)
        {
            true_sum += A[j];
            true_min = std::min(true_min, A[j]);
        }
        if(true_min != min_tree.get_min_from_range(left, right)  ||  true_sum != sum_tree.get_sum_from_range(left, right)) fail = true;
        /*std::cout << left << ' ' << right << std::endl << true_sum << ' ' << sum_tree.get_sum_from_range(left, right) << ' ' << sum_tree.get_sum_from_range(1, 8) << std::endl;
        for(int j = 0; j < A.size(); j++)  std::cout << A[j] << ' ';
        std::cout << std::endl;
        sum_tree.print_last_level();*/
    }
    if(!fail)   std::cout << "Passed system test\n";
    else        std::cout << "Failed system test\n";
    return 0;
}
