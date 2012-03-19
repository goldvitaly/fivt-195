#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <memory>

class UnaryFunc /// запретить конструктор копирования
{
    typedef unsigned int TypeNameVer;
public:
    virtual void operator()(const TypeNameVer& elem){};
    UnaryFunc(const UnaryFunc&) = delete;
    UnaryFunc() { };
private:
};

class Vertex {
    typedef unsigned int TypeNameVer;
public:
    virtual void add_neighbour(const TypeNameVer& nameVer) = 0;
    virtual void delete_neighbour(const TypeNameVer& nameVer) = 0;
    virtual bool exist_neighbour(const TypeNameVer& nameVer) = 0;
    virtual std::vector<TypeNameVer> list_neighbour() const = 0;
    virtual void for_each_neighbour(UnaryFunc& unaryFunc) const = 0;
    virtual size_t degree() const = 0;
    virtual ~Vertex(){}
private:
};


template<class StructVer>
class Graph {
    typedef unsigned int TypeNameVer;
public:
    void add_vertex(const TypeNameVer& nameVer, StructVer* structVer = new StructVer())
    {
        check_exist(nameVer, false);
        while(graph.size() < nameVer + 1)
        {
            graph.push_back(std::unique_ptr<StructVer>());
        }
        graph[nameVer].reset(structVer);
    }
    void delete_vertex(const TypeNameVer& nameVer)
    {
        check_exist(nameVer, true);
        typename std::vector<std::unique_ptr<StructVer> >::iterator it;
        for(it = graph.begin(); it != graph.end(); it++)
        {
            if(it->exist_neighbour(nameVer))
                it->delete_neighbour(nameVer);
        }
        delete graph[nameVer];
        graph[nameVer] = 0;
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
    void for_each_vertex(UnaryFunc& unaryFunc) const
    {
        for(size_t i = 0; i < graph.size(); i++)
        {
            if(graph[i] != 0)
            {
                unaryFunc(i);
            }
        }
    }
    void for_each_neighbour(const TypeNameVer& nameVer, UnaryFunc& unaryFunc) const
    {
        graph[nameVer]->for_each_neighbour(unaryFunc);
    }
    size_t size() const
    {
        return graph.size();
    }
    size_t real_size() const
    {
        size_t size = 0;
        for(size_t i = 0; i < graph.size(); i++)
        {
            if(graph[i] != 0)
                size++;
        }
        return size;
    }
private:
    std::vector<std::unique_ptr<StructVer> > graph;
    void check_exist(const TypeNameVer& nameVer, bool suppos) const
    {
        bool activity = (graph.size() > nameVer && graph[nameVer] != 0);
        if(activity != suppos)
        {
            std::cerr << "Error" << std::endl;
            exit(0);
        }
    }
};

#endif // GRAPH_H_INCLUDED
