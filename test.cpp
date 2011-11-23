#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "out_sort.h"

using namespace std;

void make_test(int n, const string& in, vector<int>& Num)
{
    Num.clear();
    srand(time(NULL));
    for(int i  = 0; i < n; i++)
        Num.push_back(rand());
    ofstream fout(in.c_str());
    for(int i = 0; i < n; i++)
        fout << Num[i] << endl;
    std::sort(Num.begin(), Num.end());
}

bool check(const int& n, const vector<int>& Num, const string& file)
{
    ifstream fin(file.c_str());
    if(Num.size() < n)
        return false;
    for(int i = 0; i < n; i++)
    {
        if(fin.eof())
            return false;
        int num_now;
        fin >> num_now;
        if(num_now != Num[i])
            return false;
    }
    return true;
}

int main()
{
    const string in = "num.txt";
    const string out = "rez.txt";
    vector<int> Num;
    const int quantity_num = 10;
    make_test(quantity_num, in, Num);
    ifstream fin(in.c_str());
    ofstream fout(out.c_str());

    out_sort<int>(fin, fout, 3, std::less<int>());

    if(check(quantity_num, Num, out))
        cout << "Ok" << endl;
    else
        cout << "Wrong" << endl;
    return 0;
}
