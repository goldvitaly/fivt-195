#include <stdio.h>
#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <time.h>

using namespace std;

template<class T, class It>
void merge_sort(It l, It r)
{
    if(r - l <= 1)
        return;
    It m = l + (r - l) / 2;
    merge_sort<T>(l, m);
    merge_sort<T>(m, r);
    vector<T> Temp(r - l);
    merge(l, m, m, r, Temp.begin());
    copy(Temp.begin(), Temp.end(), l);
}

class data{
    int key;
    int val;
public:
    explicit data(const int key_ = 0, const int val_ = 0){
        key = key_;
        val = val_;
    }
    bool operator<(const data& x) const{
        if(key < x.key)
            return true;
        else
            return false;
    }
    bool operator!=(const data& x) const{
        if(key != x.key || val != x.val)
            return true;
        else
            return false;
    }
};

void test(const int& n)
{
    srand(time(NULL));
    vector<data> A;
    for(int i = 0; i < n; i++)
    {
        int key = rand()%(n/3);
        int val = rand();
        A.push_back(data(key, val));
    }
    vector<data> B = A;
    stable_sort(A.begin(), A.end());
    merge_sort<data>(B.begin(), B.end());
    for(int i = 0; i < n; i++)
    {
        if(A[i] != B[i])
        {
            cout << "Error" << endl;
            return;
        }
    }
    cout << "Ok" << endl;
}

int main()
{
    for(int n = 10; n <= 10000; n *= 10)
        for(int trial = 0; trial < 5; trial++)
            test(n);
    return 0;
}

