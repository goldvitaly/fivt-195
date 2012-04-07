#ifndef ALGO_FOR_GRAPH_H_INCLUDED
#define ALGO_FOR_GRAPH_H_INCLUDED
#include <iostream>
#include <set>
#include <algorithm>
#include <stack>
#include "base_graph.h"
#include "graph.h"


template <class T>
class Tarjan_algo
{
public:

    explicit Tarjan_algo(Graph<T>* g)
    {
        graph_ = g;
    }


    int run()
    {
        init();
        for(int i = 0; i < graph_->size(); i++)
            if(time_in_[i] == -1)
                make_Tarjan(i);
        return num_of_strong_comp;
    }

    int get_color(int node_num)
    {
        return color_[node_num];
    }

private:
    enum
    {
        IN_STACK, NOT_IN_STACK
    };
    std::vector<int> color_;
    std::stack<int> st_;
    int time_;
    std::vector<char> in_stack_;
    std::vector<int> time_in_;
    std::vector<int> time_up_;
    std::vector<char> mark_;
    Graph<T>* graph_;
    size_t num_of_strong_comp;
    void init()
    {
        color_.clear();
        color_.resize(graph_->size(), -1);
        mark_.clear();
        mark_.resize(graph_->size(), 0);
        time_ = 0;
        in_stack_.clear();
        in_stack_.resize(graph_->size(), NOT_IN_STACK);
        time_in_.resize(graph_->size(), -1);
        time_up_.resize(graph_->size(), -1);
        num_of_strong_comp = 0;
    }

    void make_Tarjan(int curr_node)
    {
        time_in_[curr_node] = time_;
        time_up_[curr_node] = time_;
        time_++;
        in_stack_[curr_node] = IN_STACK;
        st_.push(curr_node);
        for(BaseNode::Iterator it = graph_->graph_[curr_node]->begin(); it != graph_->graph_[curr_node]->end(); ++it)
        {
            if(time_in_[*it] == -1)
            {
                make_Tarjan(*it);
                time_up_[curr_node] = std::min(time_up_[curr_node], time_up_[*it]);
            }
            else  if(in_stack_[*it] == IN_STACK)
                time_up_[curr_node] = std::min(time_up_[curr_node], time_in_[*it]);
        }
        if(time_in_[curr_node] == time_up_[curr_node])
        {
            int colored_node;
            do
            {
                colored_node = st_.top();
                st_.pop();
                color_[colored_node] = num_of_strong_comp;
                in_stack_[colored_node] = NOT_IN_STACK;
            }
            while(colored_node != curr_node);
            num_of_strong_comp++;
        }
    }
};
#endif // ALGO_FOR_GRAPH_H_INCLUDED
