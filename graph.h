#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <iostream>
#include <set>
#include <utility>
#include <map>
#include "base_graph.h"


template <class T>
class Graph
{
public:
    void add_node(int num, BaseNode* node)
    {
        graph_.insert(std::make_pair(num, node));
    }

    void delete_node(int num)
    {
        if(find(num) != graph_.end())
            graph_.erase(num);
    }

    void add_edge(int from, int to)
    {
        find(from) -> add_edge(to);
        find(to) -> add_inc_edge(from);
    }


    void add_dual_edge(int from, int to)
    {
        add_edge(from, to);
        add_edge(to, from);
    }

    void inverse()
    {
        for(typename std::map<int, BaseNode*>::iterator it = graph_.begin(); it != graph_.end(); it++)
            it -> second -> inverse_edges();
    }

    void delete_edge(int from, int to)
    {
        find(from) -> second -> delete_edge(find(to));
        find(to) -> second -> delete_inc_edge(find(from));
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

    BaseNode* find(int num)
    {
        return(graph_.find(num) -> second);
    }

    std::vector<int> get_neighbours(int node_num)
    {
        return find(node_num) -> get_neighbours();
    }

    void clear_flags()
    {
        for(typename std::map<int, BaseNode*>::iterator it = graph_.begin(); it != graph_.end(); it++)
        {
            it -> second -> flag_for_dfs_ = 0;
        }
    }



    void make_dfs(std::vector<int>* top_sorted = NULL, int* comp = NULL)    //in vector we get top sorted order of nodes
    {                                                                       //in comp we get number of components in not oriented graph
        clear_flags();
        int time = 0;
        for(typename std::map<int, BaseNode*>::iterator it = graph_.begin(); it != graph_.end(); it++)
            if(it -> second -> flag_for_dfs_ == false)
            {
                time = dfs(it -> first, time, top_sorted);
                if(comp != NULL)
                    (*comp)++;
            }
    }


private:

    int dfs(int node_num, int time, std::vector<int>* top_sorted = NULL)
    {
        find(node_num) -> flag_for_dfs_ = 1;
        find(node_num) -> time_in = time;
        std::vector<int> neighbours = get_neighbours(node_num);
        for(int i = 0; i < neighbours.size(); i++)
            if(!(find(neighbours[i]) -> flag_for_dfs_))
            {
                time = dfs(neighbours[i], time + 1, top_sorted);
            }
        find(node_num) -> time_out = time;
        if(top_sorted != NULL)
            top_sorted -> push_back(node_num);
        return time;
    }

    std::map<int, BaseNode*> graph_;
};
#endif
