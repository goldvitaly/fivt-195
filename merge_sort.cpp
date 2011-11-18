#include <stdio.h>
#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>

using namespace std;

struct type{
    int key;
    int val;
    bool operator <(type x) const{
        if(key < x.key)
            return true;
        else
            return false;
    }
};

vector <type> A;

void merge_sort(vector<type> :: iterator l, vector<type> :: iterator r)
{
    if(r - l <= 1)
        return;
    vector<type> :: iterator m = l + (r - l) / 2;
    merge_sort(l, m);
    merge_sort(m, r);
    vector<type> T(r - l);
    merge(l, m, m, r, T.begin());
    for(int i = 0; i < (r - l); i++)
        *(l + i) = *(T.begin() + i);
}

int main()
{
    int n;
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
    {
        type temp;
        cin >> temp.key >> temp.val;
        A.push_back(temp);
    }
    merge_sort(A.begin(), A.end());

    cout << endl;
    for(int i = 0; i < n; i++)
        cout << A[i].key << " " << A[i].val << endl;
    return 0;
}

