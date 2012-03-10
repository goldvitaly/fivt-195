#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

template<class T>
class UnaryFunc
{
public:
    virtual void operator()(T elem) = 0;
};

template<class TypeNameVer>
class Vertex {
public:
    virtual void add_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.insert(nameVer);
    }
    virtual void delete_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.erase(nameVer);
    }
    virtual std::vector<TypeNameVer> list_neighbour() const
    {
        std::vector<TypeNameVer> Vec;
        Vec.resize(neighbours.size());
        std::copy(neighbours.begin(), neighbours.end(), Vec.begin());
        return Vec;
    }
    virtual void for_each_neighbour(UnaryFunc<TypeNameVer>& unaryFunc)
    {
        typename std::set<TypeNameVer>::iterator it_begin = neighbours.begin();
        typename std::set<TypeNameVer>::iterator it_end = neighbours.end();
        while(it_begin != it_end)
        {
            unaryFunc(*(it_begin));
            it_begin++;
        }
    }
    virtual size_t degree() const
    {
        return neighbours.size();
    }
    virtual ~Vertex<TypeNameVer>()
    {
        neighbours.clear();
    }
private:
    std::set<TypeNameVer> neighbours;
};


template<class TypeNameVer, class StructVer>
class Graph {
public:
    void add_vertex(const TypeNameVer& nameVer, StructVer* structVer = new StructVer())
    {
        check_exist(nameVer, false);
        graph.insert(std::make_pair(nameVer, structVer));
    }
    void add_edge(const TypeNameVer& outVer, const TypeNameVer& inVer)
    {
        check_exist(outVer, true);
        check_exist(inVer, true);
        graph[outVer]->add_neighbour(inVer);
    }
    void delete_edge(const TypeNameVer& outVer, const TypeNameVer& inVer)
    {
        check_exist(outVer, true);
        check_exist(inVer, true);
        graph[outVer]->delete_neighbour(inVer);
    }
    std::vector<TypeNameVer> list_neighbour(const TypeNameVer& nameVer) const
    {
        check_exist(nameVer, true);
        return graph[nameVer]->list_neighbour();
    }
    void for_each_neighbour(const TypeNameVer& nameVer, UnaryFunc<TypeNameVer>& unaryFunc)
    {
        graph[nameVer]->for_each_neighbour(unaryFunc);
    }
    size_t size() const
    {
        return graph.size();
    }
private:
    std::map<TypeNameVer, StructVer*> graph;
    void check_exist(const TypeNameVer& nameVer, bool suppos) const
    {
        bool activity = (graph.find(nameVer) != graph.end());
        if(activity != suppos)
        {
            std::cerr << "Error" << std::endl;
            exit(0);
        }
    }
};

#endif // GRAPH_H_INCLUDED
