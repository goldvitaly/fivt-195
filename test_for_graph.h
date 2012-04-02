#ifndef TEST_FOR_GRAPH_H_INCLUDED
#define TEST_FOR_GRAPH_H_INCLUDED
#include <iostream>
#include <cstdlib>
#include "graph.h"
#include "algo_for_graph.h"

template <class T>
class Test_For_Graph
{
public:
    Test_For_Graph(Graph<T>* graph)
    {
        graph_ = graph;
    }

    void init_marks()
    {
        mark_.resize(graph_->size());
        for(int i = 0; i < graph_->size(); i++)
            mark_[i] = 0;
    }




    int run()
    {
        bool poss_x_to_y, poss_y_to_x;
        for(int i = 0; i < 50000; i++)
        {
            generate_graph();
            Tarjan_algo<T> Tarjan_for_graph_ = Tarjan_algo<T>(graph_);
            Tarjan_for_graph_.run();
            for(int x = 0; x < graph_->size(); x++)
                for(int y = x + 1; y < graph_->size(); y++)
                {
                    init_marks();
                    dfs(x);
                    poss_x_to_y = (mark_[y] != 0);
                    init_marks();
                    dfs(y);
                    poss_y_to_x = (mark_[x] != 0);
                    if((Tarjan_for_graph_.get_color(x) == Tarjan_for_graph_.get_color(y)  &&  (!poss_x_to_y  ||  !poss_y_to_x)) ||
                       (Tarjan_for_graph_.get_color(x) != Tarjan_for_graph_.get_color(y)  &&  poss_x_to_y  &&  poss_y_to_x))
                        return -1;
                }
        }
        return 0;
    }
private:

    Graph<T>* graph_;

    std::vector <char> mark_;

    void generate_graph()
    {
        graph_->graph_.clear();
        graph_->graph_.resize(1 + rand() % 10);
        for(int i = 0; i < graph_->size(); i++)
            if(i % 2)   graph_->graph_[i] = new SetNode<int>(1);
            else        graph_->graph_[i] = new VectorNode<int>(1);
        for(int i = 0; i < rand() % 100; i++)
        {
            graph_->add_edge(rand() % graph_->size(), rand() % graph_->size());
        }
    }

    void dfs(int curr_node)
    {
        mark_[curr_node] = 1;
        for(typename BaseNode::BaseIterator* it = graph_->graph_[curr_node]->begin(); *it != *(graph_->graph_[curr_node]->end()); ++(*it))
            if(!mark_[**it])
                dfs(**it);
    }
};


#endif // TEST_FOR_GRAPH_H_INCLUDED
