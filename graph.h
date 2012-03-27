#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <iostream>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include "base_graph.h"

template <class T>
class Graph
{
public:

    std::vector<BaseNode<T>* > graph_;

    void add_node(BaseNode<T>* node)
    {
        graph_.push_back(node);
    }

    void add_edge(int from, int to)
    {
        graph_[from] -> add_edge(to);
    }


    void add_dual_edge(int from, int to)
    {
        add_edge(from, to);
        add_edge(to, from);
    }

    void delete_edge(int from, int to)
    {
        graph_[from] -> delete_edge(to);
    }

    void delete_dual_edge(int from, int to)
    {
        delete_edge(from, to);
        delete_edge(to, from);
    }

    size_t size()
    {
        return graph_.size();
    }
private:
};





#endif
