#ifndef OUT_SORT_H_INCLUDED
#define OUT_SORT_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <utility>

using namespace std;

template <class Cmp>
class ReverseCmp
{
public:
    template <class T>
    bool operator () (const T& a, const T& b)
    {
        return Cmp()(b, a);
    }
};

template<class Cmp>
class CmpPairFirst
{
public:
    template<class T1, class T2>
    bool operator () (const pair<T1, T2>& a, const pair<T1, T2>& b)
    {
        return Cmp()(a.first, b.first);
    }
};

template<class T, class Cmp>
void out_sort(ifstream& from, ofstream& rez, int size_blok, Cmp cmp)
{
    int num_bloks = 0;
    char name_file[20];
    vector <T> A(size_blok);
    while(from.good())
    {
        sprintf(name_file,"%d.txt", num_bloks);
        ofstream to(name_file);
        int size_file;
        for(size_file = 0; from.good() && size_file < size_blok; )
        {
            T num_now;
            if(from >> num_now)
            {
                A[size_file++] = num_now;
            }
        }
        sort(A.begin(), A.begin() + size_file, Cmp());
        for(int j  = 0; j < size_file; j++)
            to << A[j] << endl;
        if(size_file > 0)
            num_bloks++;
    }

    priority_queue<pair<T, ifstream*>, vector<pair<T, ifstream*> >,CmpPairFirst<ReverseCmp<Cmp> > > Queue;

    for(int i = 0; i < num_bloks; i++)
    {
        sprintf(name_file,"%d.txt", i);
        ifstream* in = new ifstream(name_file);
        T num_now;
        (*in) >> num_now;
        Queue.push(make_pair(num_now, in));
    }
    while(!Queue.empty())
    {
        pair<T, ifstream*> Pair = Queue.top();
        Queue.pop();
        rez << Pair.first << endl;
        T num_now;
        if(*(Pair.second) >> num_now)
        {
            Pair.first = num_now;
            Queue.push(Pair);
        }
        else
        {
            delete Pair.second;
        }
    }
    return;
}

#endif // OUT_SORT_H_INCLUDED
