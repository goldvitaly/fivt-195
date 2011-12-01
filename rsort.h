#include <memory.h>
#include <vector>

using namespace std;


template<class _type>
void radix_sort(vector<_type> &a, int n)
{
    const int BIT = 10;
    vector<_type> sorted(n+1);
    vector<int> cnt(((1 << BIT) + 1));
    _type t;

    int i, k, z, bit = ((8 * sizeof(_type)) / BIT - 1) * BIT;



    for (k=0; k<=bit; k+=BIT)
    {
        z = (1 << (k+BIT)) - (1 << k);
        memset(&cnt[0], 0, sizeof(int) * ((1 << BIT) + 1));
        for (i=0; i<n; i++)
        {
            t = (a[i] & z) >> k;
            cnt[t]++;
        }
        for (i=1; i<=(1 << BIT); i++)
            cnt[i] += cnt[i-1];


        for (i=n-1; i>=0; i--)
        {
            t = (a[i] & z) >> k;
            cnt[t]--;
            sorted[cnt[t]] = a[i];
        }

        swap(a, sorted);
        //a.assign( sorted.begin(), sorted.end() );
    }
    cnt.clear();
    sorted.clear();

}

