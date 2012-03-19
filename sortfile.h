#include <vector>
#include <queue>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <fstream>

const int MAX_N = 100000;

int n_file = 0;
std::vector<std::string> files;

void gen_next()
{

    std::cout<< 1;
    std::string rnd = "";
    for (int i=0; i<14; i++)
        rnd += (rand() % 26 + 'a');
    rnd += ".rtf";
    if (std::ifstream(rnd.c_str()) != NULL)
    {
        gen_next();
        return;
    }
    files.push_back(rnd);
}

void print(std::vector<int> &v, int sz)
{
    std::ofstream out(files[files.size()-1].c_str());
    for (int i=0; i<sz; i++)
        out<< v[i]<<' ';
    out.close();

}

void do_files(std::string fname)
{
    std::vector<int> v(MAX_N + 5);
    std::ifstream in(fname.c_str());
    int x, sz = 0;
    while (in >> x)
    {
        if (sz == MAX_N)
        {

            v[sz] = x;
            sz++;
            std::sort(v.begin(), v.end());
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
        sort(v.begin(), v.end());
        gen_next();

        print(v, sz);
    }
    //cout<< "do_files\n";
}


void do_result()
{
    std::priority_queue< std::pair<int, std::ifstream *> > q;
    int x;
    std::ifstream *file, *temp;
    for (int i=0; i<files.size(); i++)
    {
        file = new std::ifstream(files[i].c_str());
        //assert(file -> is_open());
        *file >> x;
        std::cout<< x<<' ';
        //assert(file -> good());
        q.push(make_pair(x, file));
    }
    std::ofstream out("result.txt");
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
        remove(files[i].c_str());
    }
}

void sort_file(std::string fname)
{
    do_files(fname);

    do_result();
    del_files();
}
