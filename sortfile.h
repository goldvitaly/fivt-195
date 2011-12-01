#include <string>
#include <cassert>
#include "rsort.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <queue>
#include <time.h>

using namespace std;

const int MAX_N = 50000000;

int n_file = 0;
vector<string> files;

void gen_next()
{
    string rnd = "";
    for (int i=0; i<14; i++)
        rnd += (rand() % 26 + 'a');
    rnd += ".rtf";
    if (ifstream(rnd.c_str()) != NULL)
    {
        gen_next();
        return;
    }
    files.push_back(rnd);
}

void print(vector<int> &v, int sz)
{
    ofstream out(files[files.size()-1].c_str());
    //out<<' ';
    for (int i=0; i<sz; i++)
        out<< v[i]<<' ';
    out.close();
    //cout<< "print\n";

}

void do_files(string fname)
{
    vector<int> v(MAX_N + 5);
    ifstream in(fname.c_str());
    int x, sz = 0;
    while (in >> x)
    {
        if (sz == MAX_N)
        {
            v[sz] = x;
            sz++;
            radix_sort(v, sz);
            gen_next();
            print(v, sz);
            sz = 0;
        }
        else
        {
            v[sz] = x;
            sz++;
        }
    }

    if (sz != 0)
    {
        radix_sort(v, sz);
        gen_next();
        print(v, sz);
    }
    //cout<< "do_files\n";
}


void do_result()
{
    priority_queue< pair<int, ifstream *> > q;
    int x;
    ifstream *file, *temp;
    for (int i=0; i<files.size(); i++)
    {
        file = new ifstream(files[i].c_str());
        //assert(file -> is_open());
        *file >> x;
        //assert(file -> good());
        q.push(make_pair(x, file));
    }
    ofstream out("result.txt");
    while (!q.empty())
    {
        out<<q.top().first<<' ';
        temp = q.top().second;
        if (*temp >> x)
        {
            q.pop();
            q.push(make_pair(x, temp));
        }
        else
        {
            temp -> close();
            q.pop();
        }
    }
}
void del_files()
{
    for (int i=0; i<files.size(); i++)
    {
        //ifstream(files[i].c_str()).close();
        remove(files[i].c_str())<<' ';
    }
}

void sort_file(string fname)
{
    do_files(fname);
    do_result();
    del_files();
}
