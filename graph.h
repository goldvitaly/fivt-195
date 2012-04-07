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

    std::vector<std::unique_ptr<BaseNode> > graph_;

    void add_node(BaseNode* node)
    {
        graph_.push_back(std::unique_ptr<BaseNode>(node));
    }

    void add_edge(size_t from, size_t to)
    {
        graph_[from] -> add_edge(to);
    }

    void delete_edge(size_t from, size_t to)
    {
        graph_[from] -> delete_edge(to);
    }

    size_t size() const
    {
        return graph_.size();
    }
private:
};





#endif
