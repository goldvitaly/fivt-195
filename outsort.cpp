#include <cstdlib>
#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>
#include <cstring>
#include <string>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <typeinfo>
#include <functional>

using namespace std;

string crTmpFile(int k)
{
    std::stringstream buf;
    string output1 = "tmp";
    string output2 = ".out";
    buf.clear();
    buf << k;
    string num;
    buf >> num;
    string FileName = output1 + num + output2;
    return FileName;
}

template <typename typeElem>
bool cmpfunc(const typeElem &a, const typeElem &b)
{
    return a > b;
}

template <typename typeElem, class Compare>
void OutSort(string inputName, string outputName, Compare cmpElem) // передаем имя входного, выходного файлов и переменную сортируемого типа данных
{
//Открываем входной и выходной файлы
    ifstream in;
    ofstream res;
    in.open(inputName, ifstream::in);
    res.open(outputName, ofstream::out);
    //const int maxsize = 30; // for debugging
    const int maxsize = 50000000; // максимальное количество байт в памяти, которое мы можем использовать (с маленькой погрешностью)

    typedef pair <typeElem, fstream *> ourPair;
    vector< pair <typeElem, string> > files;

//чтение данных, их обработка в временные файлы, в которых числа отсортированы
    while (in.good())
    {
        size_t cursize = 0;
        vector<typeElem> block;
        while (cursize < maxsize && in.good())
        {
            typeElem readElem;
            in >> readElem;
            block.push_back(readElem);
            cursize += sizeof(readElem);
        }
        if (!in.good())
            block.pop_back();
        sort(block.begin(), block.end(), cmpElem);
        string FileName = crTmpFile(files.size());
        fstream out;
        out.open(FileName, fstream::out);
        files.push_back(make_pair(block[0], FileName));
        for (size_t i = 0; i < block.size(); i++)
            out << block[i] << endl;
        out.close();
    }
    in.close();

//создаем кучу
    auto compQueue = [cmpElem](const ourPair &a, const ourPair &b ) { return cmpElem(b.first, a.first); };
    priority_queue <ourPair, vector<ourPair>, decltype(compQueue) > q(compQueue);
    for (size_t i = 0; i < files.size(); i++)
    {
        typeElem elem;
        fstream * tmpFile = new fstream;
        (*tmpFile).open(files[i].second, fstream::in);
        (*tmpFile) >> elem;
        q.push(make_pair(elem, tmpFile));
    }

//производим сортировку слиянием с помощью кучи
    while (!q.empty())
    {
        ourPair top = q.top(), newElem;
        q.pop();
        res << top.first << " ";
        fstream * curFile = newElem.second = top.second;
        (*curFile) >> ws;
        if ((*curFile).good())
        {
            (*curFile) >> newElem.first;
            q.push(newElem);
        }
        else
            (*curFile).close();
    }
    res.close();
// стираем временные файлы
    for (size_t i = 0; i < files.size(); i++)
        remove(files[i].second.c_str());
}

// тестируем нашу МЕГАпрогу
const int N = 100000; // количество элементов, которое будем сортировать
void testint()
{
    srand(time(NULL));
    ofstream in;
    ifstream out;
    in.open("testint.in", ifstream::out);
    for (int i = 0; i < N; i++)
        in << rand() << " ";
    in.close();
    OutSort<int>("testint.in", "testint.out", greater<int>());
    out.open("testint.out", ofstream::in);
    bool res = true;
    int pr, y;
    out >> pr;
    for (int i = 1; i < N; i++)
        if (out.bad())
        {
            res = false;
            break;
        }
        else
        {
            out >> y;
            if (y > pr)
            {
                res = false;
                break;
            }
            pr = y;
        }
    cout << res;
    out.close();
}

int main()
{
    testint();
    //OutSort<int>("testint.in", "testint.out", greater<int>());
    return 0;
}
