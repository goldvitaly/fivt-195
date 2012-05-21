#ifndef TEST_MAX_FLOW_H_INCLUDED
#define TEST_MAX_FLOW_H_INCLUDED

#include "max_flow.h"


template<typename Weight, typename CompPath = std::greater<Weight> >
class Test_For_Graph
{
public:
    explicit Test_For_Graph(Graph<Weight>* graph)
    {
        graph_ = graph;
    }



    void run()
    {
        for(int test_num = 0; test_num < 5000; test_num++)
        {
            generate_graph();

            for(int i = 0; i < rand() % 20; i++)
            {
                MaxFlowFinder<Weight, CompPath> prev_flow(*graph_, 0);
                int x = rand();
                int y = rand();
                int prev = prev_flow.calculate_flow(x, y);
                prev += add_random_way(x, y);
                std::cout << prev;
                return;
                MaxFlowFinder<Weight, CompPath> new_flow(*graph_, 0);
                if(new_flow.calculate_flow(x, y) != prev)
                    return;
            }

        }
        printf("OK\n");
        return;
    }
private:

    Graph<Weight>* graph_;
    void generate_graph()
    {
        graph_->graph_.clear();
        graph_->graph_.resize(1 + rand() % 250);
        for(int i = 0; i < graph_->size(); i++)
            if(i % 2)   graph_->graph_[i] = std::unique_ptr<SetNode<Weight> >(new SetNode<Weight>);
            else        graph_->graph_[i] = std::unique_ptr<VectorNode<Weight> >(new VectorNode<Weight>);
        for(int i = 0; i < rand() % 10000; i++)
        {
            graph_->add_edge(rand() % graph_->size(), rand() % graph_->size(), rand() % 1000);
        }
    }

    int add_random_way(int from, int to)
    {
        int weight = rand();
        if(from < to)
        {
            for(int i = from + 1; i < to; i++)
                if(rand() % 2)
                {
                    graph_->add_edge(from, i, weight);
                    from = i;
                }
        }
        else
        {
            for(int i = to; i > from; i--)
                if(rand() % 2)
                {
                    graph_->add_edge(i, to, weight);
                    to = i;
                }
        }
        graph_->add_edge(from, to, weight);
        return weight;
    }
};


#endif // TEST_MAX_FLOW_H_INCLUDED
