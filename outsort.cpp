#include <cstdlib>
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
#include <ctime>
#include <istream>
#include <ostream>
#include <iostream>

std::string createTmpFile(int k)
{
    std::stringstream buf;
    std::string pref = "tmp";
    std::string suf = ".out";
    buf.clear();
    buf << k;
    std::string num;
    buf >> num;
    std::string fileName = pref + num + suf;
    return fileName;
}

//for testing
template <typename TypeElem>
bool cmpfunc(const TypeElem &a, const TypeElem &b)
{
    return a > b;
}

template <typename TypeElem, class Compare>
void OutSort(const std::string &inputName, const std::string &outputName, Compare cmpElem) // передаем имя входного, выходного файлов и переменную сортируемого типа данных
{
//open input file and output file
    std::ifstream in;
    std::ofstream res;
    in.open(inputName, std::ifstream::in);
    res.open(outputName, std::ofstream::out);
    //const int maxsize = 30; // for debugging
    const int maxSize = 50000000; // maximal memory, which we can use

    typedef std::pair <TypeElem, std::fstream *> mergePair;
    std::vector< std::pair <TypeElem, std::string> > files;

//read data, create tmp files
    while (in.good())
    {
        size_t cursize = 0;
        std::vector<TypeElem> block;
        while (cursize < maxSize && in.good())
        {
            TypeElem readElem;
            in >> readElem;
            block.push_back(readElem);
            cursize += sizeof(readElem);
        }
        if (!in.good())
            block.pop_back();
        std::sort(block.begin(), block.end(), cmpElem);
        std::string fileName = createTmpFile(files.size());
        std::fstream out;
        out.open(fileName, std::fstream::out);
        files.push_back(make_pair(block[0], fileName));
        for (size_t i = 0; i < block.size(); i++)
            out << block[i] << std::endl;
        out.close();
    }
    in.close();

//create queue
    auto compQueue = [cmpElem](const mergePair &a, const mergePair &b ) { return cmpElem(b.first, a.first); };
    std::priority_queue <mergePair, std::vector<mergePair>, decltype(compQueue) > q(compQueue);
    for (size_t i = 0; i < files.size(); i++)
    {
        TypeElem elem;
        std::fstream * tmpFile = new std::fstream;
        (*tmpFile).open(files[i].second, std::fstream::in);
        (*tmpFile) >> elem;
        q.push(make_pair(elem, tmpFile));
    }

//merging with queue
    while (!q.empty())
    {
        mergePair top = q.top(), newElem;
        q.pop();
        res << top.first << " ";
        std::fstream * curFile = newElem.second = top.second; // add next element from file top.second in queue
        (*curFile) >> std::ws;
        if ((*curFile).good())
        {
            (*curFile) >> newElem.first;
            q.push(newElem);
        }
        else
            (*curFile).close();
    }
    res.close();
    for (size_t i = 0; i < files.size(); i++)
        remove(files[i].second.c_str());
}

// testing
const int N = 100000; // quantity of elements, which we will sort
void testInt()
{
    srand(time(NULL));
    std::ofstream in;
    std::ifstream out;
    in.open("testint.in", std::ifstream::out);
    for (int i = 0; i < N; i++)
        in << rand() << " ";
    in.close();
    OutSort<int>("testint.in", "testint.out", std::greater<int>());
    out.open("testint.out", std::ofstream::in);
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
    std::cout << res;
    out.close();
}

int main()
{
    testInt();
    //OutSort<int>("testint.in", "testint.out", greater<int>());
    return 0;
}
