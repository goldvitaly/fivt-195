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
    bool operator () (const std::pair<T1, T2>& a, const std::pair<T1, T2>& b)
    {
        return Cmp()(a.first, b.first);
    }
};

template<class T, class Cmp>
void out_sort(std::ifstream& from, std::ofstream& res, int size_blok, Cmp cmp)
{
    int num_bloks = 0;
    char name_file[20];
    std::vector<T> Data(size_blok);
    while(from.good())
    {
        sprintf(name_file,"%d.txt", num_bloks);
        std::ofstream to(name_file);
        int size_file;
        for(size_file = 0; from.good() && size_file < size_blok; )
        {
            T num_now;
            if(from >> num_now)
            {
                Data[size_file++] = num_now;
            }
        }
        std::sort(Data.begin(), Data.begin() + size_file, Cmp());
        for(int j  = 0; j < size_file; j++)
            to << Data[j] << std::endl;
        if(size_file > 0)
            num_bloks++;
    }

    typedef CmpPairFirst<ReverseCmp<Cmp> > CmpQueue;
    std::priority_queue<std::pair<T, std::ifstream*>, std::vector<std::pair<T, std::ifstream*> >, CmpQueue> Queue;

    for(int i = 0; i < num_bloks; i++)
    {
        sprintf(name_file,"%d.txt", i);
        std::ifstream* in = new std::ifstream(name_file);
        T num_now;
        (*in) >> num_now;
        Queue.push(make_pair(num_now, in));
    }
    while(!Queue.empty())
    {
        std::pair<T, std::ifstream*> Pair = Queue.top();
        Queue.pop();
        res << Pair.first << std::endl;
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
