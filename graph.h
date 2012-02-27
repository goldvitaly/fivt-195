#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <iostream>
#include <set>
#include <utility>
#include <map>
template <class T>
class Graph
{
public:
    class Node
    {
    private:
        T value_;

    public:
        std::map <Node*, int> inc_edges_, out_edges_;  //int - weight of edge

        int color_, time_in_, time_out_;\

        Node(T value = 0)
        {
            color_ = 0;
            value_ = value;
            time_in_ = 0;
            time_out_ = 0;
        }

        size_t num_of_inc_edges() const
        {
            return(inc_edges_.size());
        }

        size_t num_of_out_edges() const
        {
            return(out_edges_.size());
        }
    };

    std::map<int, Node*> graph_;

    void add_node(int num, T value = 0)
    {
        graph_.insert(std::make_pair(num, new Node(value)));
    }

    void add_edge(int from, int to, int weight = 1)
    {
        graph_.find(from) -> second -> out_edges_.insert(std::make_pair(graph_.find(to) -> second, weight));
        graph_.find(to) -> second -> inc_edges_.insert(std::make_pair(graph_.find(from) -> second, weight));
    }


    void add_dual_edge(int from, int to, int weight = 1)
    {
        add_edge(from, to, weight);
        add_edge(to, from, weight);
    }

    void inverse()
    {
        for(typename std::map<int, Node*>::iterator it = graph_.begin(); it != graph_.end(); it++)
            it -> second -> inc_edges_.swap(it -> second -> out_edges_);
    }

    Node* find(int num)
    {
        return(graph_.find(num) -> second);
    }
};

#endif // GRAPH_H_INCLUDED
