#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <iostream>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include "base_graph.h"

template <class Weight>
class Path
{
public:

    typedef typename BaseNode<Weight>::Iterator Iterator;
    std::vector<Iterator> path_;
    explicit Path(size_t direction): direction_(direction) {}

    void add(Iterator new_edge)
    {
        path_.push_back(std::move(new_edge));
    }

    void set_start(size_t start)
    {
        start_ = start;
    }


    void print()
    {
        std::cout << start_;
        for(int i = 0; i < path_.size(); i++)
            std::cout << ' ' << **path_[i];
        std::cout << std::endl;
    }

    void reverse_path()
    {
        reverse(path_.begin(), path_.end());
    }

    size_t size() const
    {
        return path_.size();
    }
private:

    size_t direction_, start_;
};




template <class Weight>
class Graph
{
public:

    std::vector<std::unique_ptr<BaseNode<Weight> > > graph_;

    void add_node(BaseNode<Weight>* node)
    {
        graph_.push_back(std::unique_ptr<BaseNode<Weight>>(node));
    }

    void add_edge(size_t from, size_t to, Weight weight, size_t id = 0)
    {
        graph_[from] -> add_edge(Edge<Weight>(to, weight, id));
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
