#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <iostream>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include "base_graph.h"

template <class Data, class Weight>
class Graph
{
public:

    std::vector<std::unique_ptr<BaseNode<Weight> > > graph_;

    void add_node(BaseNode<Weight>* node)
    {
        graph_.push_back(std::unique_ptr<BaseNode<Weight>>(node));
    }

    void add_edge(size_t from, size_t to, Weight weight)
    {
        graph_[from] -> add_edge(Edge<Weight>(to, weight));
    }

    void delete_edge(size_t from, size_t to, Weight weight)
    {
        graph_[from] -> delete_edge(Edge<Weight>(to, weight));
    }

    size_t size() const
    {
        return graph_.size();
    }
private:
};





#endif
