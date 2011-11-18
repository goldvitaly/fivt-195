#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <utility>
#include <set>
#include <sstream>

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
class Cmp_pair
{
public:
    template<class T1, class T2>
    bool operator () (pair<T1, T2> a, pair<T1, T2> b)
    {
        return Cmp()(a.first, b.first);
    }
};

template<class T, class Cmp>
void out_sort(ifstream& from, ofstream& rez, int size_bl = 1000, Cmp cmp)
{
    int num_bloks = 0;
    char name_fail[20];
    vector <T> A(size_bl);
    while(!from.eof())
    {
        sprintf(name_fail,"%d.txt", num_bloks);
        ofstream to(name_fail);
        int size_fail;
        for(size_fail = 0; !from.eof() && size_fail < size_bl; size_fail++)
        {
            T num_now;
            from >> num_now;
            A[size_fail] = num_now;
        }
        sort(A.begin(), A.begin() + size_fail, Cmp());

        for(int j  = 0; j < size_fail; j++)
            to << A[j] << endl;
        num_bloks++;
    }

    std::priority_queue <
		std::pair < T, std::ifstream*>,
		std::vector < std::pair < T, std::ifstream* > >,
		Cmp_pair<ReverseCmp<Cmp> >
	> Queue;


    for(int i = 0; i < num_bloks; i++)
    {
        sprintf(name_fail,"%d.txt", i);
        ifstream* in = new ifstream(name_fail);
        T num_now;
        (*in) >> num_now;
        Queue.push(make_pair(num_now, in));
    }
    while(!Queue.empty())
    {
        pair<T, ifstream*> Pair = Queue.top();
        Queue.pop();
        rez << Pair.first << endl;
        if(!(*Pair.second).eof())
        {
            T num_now;
            *(Pair.second) >> num_now;
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

int main()
{
    ifstream from("num.txt");
    ofstream out("rez.txt");
    out_sort<int>(from, out, 1000, std::less<int>());
    return 0;
}

