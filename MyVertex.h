#ifndef MYVERTEX_H_INCLUDED
#define MYVERTEX_H_INCLUDED

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include "graph.h"
#include <ctime>

template<class TypeNameVer>
class MyVertex : public Vertex<TypeNameVer> {
public:
    void add_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.push_back(nameVer);
    }
    void delete_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.erase(find(neighbours.begin(), neighbours.end(), nameVer));
    }
    std::vector<TypeNameVer> list_neighbour() const
    {
        std::vector<TypeNameVer> Vec(neighbours);
        return Vec;
    }
    void for_each_neighbour(UnaryFunc<TypeNameVer>& unaryFunc)
    {
        typename std::vector<TypeNameVer>::iterator it_begin = neighbours.begin();
        typename std::vector<TypeNameVer>::iterator it_end = neighbours.end();
        while(it_begin != it_end)
        {
            unaryFunc(*(it_begin));
            it_begin++;
        }
    }
    size_t degree() const
    {
        return neighbours.size();
    }
    ~MyVertex()
    {
        neighbours.clear();
    }
private:
    std::vector<TypeNameVer> neighbours;
};


#endif // MYVERTEX_H_INCLUDED
