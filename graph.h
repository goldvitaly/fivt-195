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
private:
};

template<class TypeNameVer>
class Vertex {
public:
    virtual void add_neighbour(const TypeNameVer& nameVer) = 0;
    virtual void delete_neighbour(const TypeNameVer& nameVer) = 0;
    virtual bool exist_neighbour(const TypeNameVer& nameVer) = 0;
    virtual std::vector<TypeNameVer> list_neighbour() const = 0;
    virtual void for_each_neighbour(UnaryFunc<TypeNameVer>& unaryFunc) = 0;
    virtual size_t degree() const = 0;
    virtual ~Vertex(){}
private:
};


template<class TypeNameVer, class StructVer>
class Graph {
public:
    void add_vertex(const TypeNameVer& nameVer, StructVer* structVer = new StructVer())
    {
        check_exist(nameVer, false);
        graph.insert(std::make_pair(nameVer, structVer));
    }
    void delete_vertex(const TypeNameVer& nameVer)
    {
        check_exist(nameVer, true);
        typename std::map<TypeNameVer, StructVer*>::iterator it;
        for(it = graph.begin(); it != graph.end(); it++)
        {
            if(it->second->exist_neighbour(nameVer))
                it->second->delete_neighbour(nameVer);
        }
        delete graph[nameVer];
        graph.erase(nameVer);
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
    ~Graph()
    {
        typename std::map<TypeNameVer, StructVer*>::iterator it = graph.begin();
        while(it != graph.end())
        {
            delete it->second;
            it++;
        }
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
