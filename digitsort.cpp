#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

typedef unsigned SortInt;

template<typename t>
class SortClass {};

template<>
class SortClass<string>
{
    public:
        SortInt getMax(int)
        {
            return 255;
        }
        template <typename Iterator>
        SortInt getKey(Iterator it, int pos)
        {
            if (pos < (*it).size())
                return (SortInt)(*it)[pos];
            return 0;
        }

        template <typename Iterator>
        int getKeyCount(Iterator begin, Iterator end)
        {
            int maxLen = 0;
            for (Iterator it = begin; it != end; it++)
                maxLen = max(maxLen, (int)(*it).size());
            return maxLen;
        }
};

template<>
class SortClass<long long>
{
    public:
        const static int BlocksCount = 4, BitesCount = 60;
        const static int BlockSize = BitesCount / BlocksCount;
        SortInt getMax(int)
        {
            return (1ll << BlockSize);
        }
        template <typename Iterator>
        SortInt getKey(Iterator it, int pos)
        {
            long long n = *it + (1ll << (BitesCount - 1));
            n >>= BlockSize * (BlocksCount - 1 - pos);
            n = n & ((1 << BlockSize) - 1);
            return int(n);
        }

        template <typename Iterator>
        int getKeyCount(Iterator begin, Iterator end)
        {
            return BlocksCount;
        }
};

template<>
class SortClass<int>
{
    public:
        const static int BlocksCount = 2, BitesCount = 30;
        const static int BlockSize = BitesCount / BlocksCount;
        SortInt getMax(int)
        {
            return (1 << BlockSize);
        }
        template <typename Iterator>
        SortInt getKey(Iterator it, int pos)
        {
            int n = *it + (1 << (BitesCount - 1));
            n >>= BlockSize * (BlocksCount - 1 - pos);
            n = n & ((1 << BlockSize) - 1);
            return int(n);
        }

        template <typename Iterator>
        int getKeyCount(Iterator begin, Iterator end)
        {
            return BlocksCount;
        }
};



template<>
class SortClass<pair <int, int>>
{
    public:
        const static int BlocksCount = 2, BitesCount = 30;
        const static int BlockSize = BitesCount / BlocksCount;
        SortInt getMax(int)
        {
            return (1 << BlockSize);
        }
        template <typename Iterator>
        SortInt getKey(Iterator it, int pos)
        {
            if (pos > 1)
            {
                int n = (*it).second + (1 << (BitesCount - 1));
                n >>= BlockSize * (BlocksCount - 1 - pos);
                n = n & ((1 << BlockSize) - 1);
                return n;
            }
            else
            {
                int n = (*it).first + (1 << (BitesCount - 1));
                n >>= BlockSize * (BlocksCount - 1 - pos);
                n = n & ((1 << BlockSize) - 1);
                return n;
            }
        }

        template <typename Iterator>
        int getKeyCount(Iterator begin, Iterator end)
        {
            return BlocksCount * 2;
        }
};

template <typename Iterator, typename SortClass = SortClass< __typeof__ (*Iterator())> >
void radix_sort(Iterator begin, Iterator end, SortClass Sort = SortClass())
{
    vector<__typeof__ (*begin)> Temp(end - begin);
    for (int pos = Sort.getKeyCount(begin, end) - 1; pos >= 0; pos--)
    {
        int maxval = Sort.getMax(pos);
        vector<int> basket(maxval + 2);
        for (Iterator it = begin; it != end; it++)
            basket[Sort.getKey(it, pos) + 1]++;
        for (int i = 1; i <= maxval; i++)
            basket[i] += basket[i - 1];
        for (Iterator it = begin; it != end; it++)
            Temp[basket[Sort.getKey(it, pos)]++] = *it;
        auto itTmp = Temp.begin();
        for (Iterator it = begin; it != end; itTmp++, it++)
            *it= *itTmp;
    }
}

const int maxnint = 100000;
int e[maxnint], f[maxnint];
void test_int()
{
    int maxn = maxnint;
    for (int i = 0; i < maxn; i++)
        f[i] = e[i] = (((rand() & 2) - 1) * rand())  % (1 << 29);

    cout << "Sorting " << maxn << " Int" << endl;
    double t1 = clock();
    sort(e, e + maxn);
    double t2 = clock();
    double dt1 = (t2 - t1) / CLOCKS_PER_SEC;
    cout << "StdSort's time is " << dt1 << endl;

    t1 = clock();
    radix_sort(f, f + maxn);
    t2 = clock();
    double dt2 = (t2 - t1) / CLOCKS_PER_SEC;
    cout << "RadixSort's time is " << dt2 << endl;


    for (int i = 0; i < maxn; i++)
        if (e[i] != f[i])
            cout << "Error: " << i << " " << e[i] << " " << f[i] << endl;

}

const int maxnpair = 50000000;
pair<int, int> g[maxnpair], h[maxnpair];
void test_pair()
{
    int maxn = maxnpair;
    for (int i = 0; i < maxn; i++)
    {
        h[i].second = g[i].second = (((rand() & 2) - 1) * rand())  % (1 << 29);
        h[i].first = g[i].first = (((rand() & 2) - 1) * rand())  % (1 << 29);
    }

    cout << "Sorting " << maxnpair << " Pair<Int, Int>" << endl;
    double t1 = clock();
    sort(g, g + maxn);
    double t2 = clock();
    double dt1 = (t2 - t1) / CLOCKS_PER_SEC;
    cout << "StdSort's time is " << dt1 << endl;

    t1 = clock();
    radix_sort(h, h + maxn);
    t2 = clock();
    double dt2 = (t2 - t1) / CLOCKS_PER_SEC;
    cout << "RadixSort's time is " << dt2 << endl;


    for (int i = 0; i < maxn; i++)
        if (g[i] != h[i])
            cout << "Error: "<< i << " " << g[i].first << " " << h[i].first << endl;
}

const int maxnll = 50000000;
long long a[maxnll], b[maxnll];
void test_long()
{
    int maxn = maxnll;
    for (int i = 0; i < maxn; i++)
        b[i] = a[i] = (((rand() & 2) - 1) * (long long)rand() * (long long)rand()) % (1ll << 56);

    cout << "Sorting " << maxnll << " Long Long" << endl;
    double t1 = clock();
    sort(a, a + maxn);
    double t2 = clock();
    double dt1 = (t2 - t1) / CLOCKS_PER_SEC;
    cout << "StdSort's time is " << dt1 << endl;

    t1 = clock();
    radix_sort(b, b + maxn);
    t2 = clock();
    double dt2 = (t2 - t1) / CLOCKS_PER_SEC;
    cout << "RadixSort's time is " << dt2 << endl;


    for (int i = 0; i < maxn; i++)
        if (a[i] != b[i])
            cout << "Error: "<< i << " " << a[i] << " " << b[i] << endl;

}

const int maxnst = 10000000;
string c[maxnst], d[maxnst];
void test_string(int maxl)
{
    int maxn = maxnst;
    char ch;
    for (int i = 0; i < maxn; i++)
    {
        for (int j = 0; j < maxl; j++)
        {
            ch = char(rand() % 128);
            c[i] += ch;
            d[i] += ch;
        }
    }
    cout << "Sorting " << maxnst << " String " << "their length is " << maxl << endl;
    double t1 = clock();
    sort(c, c + maxn);
    double t2 = clock();
    double dt1 = (t2 - t1) / CLOCKS_PER_SEC;
    cout << "StdSort's time is " << dt1 << endl;

    t1 = clock();
    radix_sort(d, d + maxn);
    t2 = clock();
    double dt2 = (t2 - t1) / CLOCKS_PER_SEC;
    cout << "RadixSort's time is " << dt2 << endl;


    for (int i = 0; i < maxn; i++)
        if (c[i] != d[i])
            cout << "Error: " << i << " " << c[i] << " " << d[i] << endl;

}

int main()
{
    test_int();
    //test_long();
    //test_string(4);
    //test_pair();
    return 0;
}
