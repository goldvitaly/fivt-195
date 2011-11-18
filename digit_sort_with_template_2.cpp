#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>
#include <math.h>
#include <set>
#include <map>

using namespace std;

template <class T>
void prov(T l, T r)
{
    for(T j = l + 1; j < r; j++)
    {
        if((*j) < *(j - 1))
        {
            cout << "No" << endl;
            return;
        }
    }
    cout << "Yes" << endl;
}

class int_BitExt{
public:
    int_BitExt(int Bits_Num = 8){Bits_Num_ = Bits_Num;}

    int extract(unsigned int elem, int poz){
        return (elem >> poz) & ( (1<<Bits_Num_) - 1);
    }
    int size(){return 32;}
    int max_key(){return (1LL << Bits_Num_);}
    int blok() {return Bits_Num_;}
private:
    int Bits_Num_;
};

class string_BitExt{
public:
    string_BitExt(int len, int Bits_Num = 1){Bits_Num_ = Bits_Num; len_ = len;}

    int extract(string elem, int poz){
        int key = 0;
        int pow = 1;
        for(int j = len_ - poz - 1; j > len_ - poz - 1 - Bits_Num_; j--)
        {
            if(j >= 0)
            {
                key += elem[j]*pow;
                pow *= 256;
            }
            else
                key += 0;
        }
        return key;
    }
    int size(){return len_;}
    int max_key(){
        int key = 1;
        for(int j = 0; j < Bits_Num_; j++)
            key *= 256;
        return key;
    }
    int blok() {return Bits_Num_;}
private:
    int Bits_Num_;
    int len_;
};

class long_long_BitExt{
public:
    long_long_BitExt(int Bits_Num){Bits_Num_ = Bits_Num;}
    long_long_BitExt(){Bits_Num_ = 8;}

    int extract(unsigned long long elem, int poz){
        return (elem >> poz) & ((1<<Bits_Num_) - 1);
    }
    int size() {return 64;}
    int blok() {return Bits_Num_;}
    int max_key(){return (1LL << Bits_Num_);}
private:
    int Bits_Num_;
};

class pair_BitExt{
public:
    pair_BitExt(int Bits_Num){Bits_Num_ = Bits_Num;}
    pair_BitExt(){Bits_Num_ = 8;}

    int extract(pair<unsigned int, unsigned int> elem, int poz){
        unsigned long long elem2 = ((unsigned long long)elem.first << 32) + elem.second;
        return (elem2 >> poz) & ((1<<Bits_Num_) - 1);
    }
    int size() {return 64;}
    int blok() {return Bits_Num_;}
    int max_key(){return (1LL << Bits_Num_);}
private:
    int Bits_Num_;
};


template <class Type, class T, class BlokExtract>
void radix_sort( T l, T r, BlokExtract Ext)
{
    vector<int> Vs(Ext.max_key() + 1);
    vector<Type> ans(r-l);

    T L[2], R[2];
    L[0] = l;
    R[0] = r;
    L[1] = ans.begin();
    R[1] = ans.end();
    for(size_t i = 0, num_bl = 0; i < Ext.size(); i += Ext.blok(), num_bl++)
    {
        for(vector<int> :: iterator j = Vs.begin(); j != Vs.end(); j++)
            (*j) = 0;

       // memset(Vs, 0, sizeof(Vs));
        T start = L[num_bl%2], end = R[num_bl % 2];
        for(T j = start; j < end; j++)
        {
            int blok = Ext.extract(*j, i);
                Vs[blok + 1]++;
        }
        for(vector<int> :: iterator j = Vs.begin() + 1; j != Vs.end(); j++)
            (*j) += *(j-1);
        T outStart = L[(num_bl + 1)%2];
        for(T j = start; j < end; j++)
        {
            int blok = Ext.extract(*j, i);
            *(outStart + Vs[blok]++) = *j;
        }
    }
    if(((Ext.size() - 1)/ Ext.blok() + 1) % 2 == 1)
    {
        for(T j = L[1], j2 = L[0]; j < R[1]; j++, j2++)
        {
            (*j2) = (*j);
        }
    }
}

void test_int(int n, int bit = 8)
{
    srand(13);
    double START, FINISH;
    vector<unsigned int> A(n);
    vector<unsigned int> B(n);
    for(int i = 0; i < n; i++)
    {
        A[i] = (rand() << 16) + rand();
    }
    cout << "make test finish" << endl;
    B = A;
    START = clock();
    radix_sort<unsigned int>(A.begin(), A.end(), int_BitExt(bit));
    FINISH = clock();
    cout.precision(10);
    cout << "n = " << n << endl;
    cout << "unsigned int:" << endl;
    cout << "radix_sort = " << (FINISH - START) / CLOCKS_PER_SEC << endl;
    START = clock();
    sort(B.begin(), B.end());
    FINISH = clock();
    cout << "stl_sort =   " << (FINISH - START) / CLOCKS_PER_SEC << endl;
    A.clear();
    B.clear();
}

void test_long_long(int n, int bit = 8)
{
    srand(100);
    vector<unsigned long long> A(n), B(n);
    for(int i = 0; i < n; i++)
    {
        A[i] = ((unsigned long long)rand() << 48) + ((unsigned long long)rand() << 32) + ((unsigned long long)rand() << 16) + rand();
    }
    cout << "finish make test" << endl;
    B = A;
    cout << "n = " << n <<  endl;
    cout << "unsigned long long: " << endl;
    double START = clock();
    radix_sort<unsigned long long>(A.begin(), A.end(), long_long_BitExt(bit));
    double FINISH = clock();
    cout.precision(10);
    cout << "radix_sort = " << (FINISH - START) / CLOCKS_PER_SEC << endl;
    START = clock();
    sort(B.begin(), B.end());
    FINISH = clock();
    cout << "stl_sort =   " << (FINISH - START) / CLOCKS_PER_SEC << endl;
    A.clear();
    B.clear();
}

void test_pair(int n, int bit = 8)
{
    srand(0);
    vector<pair<unsigned int, unsigned int> > A(n), B(n);
    for(int i = 0; i < n; i++)
    {
        A[i].first = ((unsigned int)rand() << 16) + rand();
        A[i].second = ((unsigned int)rand() << 16) + rand();
    }
    cout << "finish make test" << endl;
    B = A;
    cout << "n = " << n <<  endl;
    cout << "pair: " << endl;
    double START = clock();
    radix_sort<pair<unsigned int, unsigned int> >(A.begin(), A.end(), pair_BitExt(bit));
    double FINISH = clock();
    prov(A.begin(), A.end());
    cout.precision(10);
    cout << "radix_sort = " << (FINISH - START) / CLOCKS_PER_SEC << endl;
    START = clock();
    sort(B.begin(), B.end());
    FINISH = clock();
    cout << "stl_sort =   " << (FINISH - START) / CLOCKS_PER_SEC << endl;
    A.clear();
    B.clear();
}

void test_string(int n, int len = 5, int bits = 1)
{
    srand(0);
    vector<string> A(n), B(n);
    string sim = "qwertyuiopasdfghjklzxcvbnm";
    for(int i = 0; i < n; i++)
    {
        A[i].resize(len);
        for(int j = 0; j < len; j++)
            A[i][j] = sim[rand()%26];
    }
    cout << "finish make test" << endl;
    B = A;
    cout << "n = " << n << endl;
    cout << "string:" << endl;
    double START = clock();
    radix_sort<string>(A.begin(), A.end(), string_BitExt(len, bits));
    double FINISH = clock();
    cout.precision(10);
    cout << "radix_sort = " << (FINISH - START) / CLOCKS_PER_SEC << endl;
    START = clock();
    sort(B.begin(), B.end());
    FINISH = clock();
    cout << "stl_sort =   " << (FINISH - START) / CLOCKS_PER_SEC << endl;
    A.clear();
    B.clear();
}

int main()
{
    int n = 10000000; //10**7
    int m, k;
    test_int(n, 8);
    test_long_long(n, 8);
    test_pair(n, 8);
    test_string(2*n/5, 3, 2);

    return 0;
}
