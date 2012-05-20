#ifndef TEST_SHORT_PATHS_H_INCLUDED
#define TEST_SHORT_PATHS_H_INCLUDED
#include "shortest_paths_finder.h"


template<typename Data, typename Weight, typename PathLen = Weight, typename CalcPath = std::plus<PathLen>, typename CompPath = std::less<PathLen> >
class Test_For_Graph
{
public:
    explicit Test_For_Graph(Graph<Data, Weight>* graph)
    {
        graph_ = graph;
    }




    void run(PathLen empty_path = PathLen())
    {
        for(int test_num = 0; test_num < 500; test_num++)
        {
            generate_graph();
            simple_shortest_paths.clear();
            simple_shortest_paths.resize(graph_->size());
            for(int i = 0; i < graph_->size(); i++)
            {
                simple_shortest_paths[i].resize(graph_->size());
                BaseNode<Weight>* curr_node = graph_->graph_[i].get();
                for(typename BaseNode<Weight>::Iterator it = curr_node->begin(); it != curr_node->end(); ++it)
                {
                    PathLen new_len = calc_path_(empty_path, (*it).get_weight());
                    if(!simple_shortest_paths[i][**it]  ||  comp_path_(new_len, simple_shortest_paths[i][**it].get()))
                        simple_shortest_paths[i][**it] = new_len;
                }
                simple_shortest_paths[i][i] = PathLen();
            }
            for(int k = 0; k < graph_->size(); k++)
                for(int i = 0; i < graph_->size(); i++)
                    for(int j = 0; j < graph_->size(); j++)
                        if(simple_shortest_paths[i][k]  &&  simple_shortest_paths[k][j])
                        {
                            PathLen new_len = calc_path_(simple_shortest_paths[i][k].get(), simple_shortest_paths[k][j].get());
                            if(!simple_shortest_paths[i][j]  ||  comp_path_(new_len, simple_shortest_paths[i][j].get()))
                                simple_shortest_paths[i][j] = new_len;
                        }
            ShortestPathFinder<Data, Weight, PathLen, CalcPath, CompPath> my_shortest_paths(*graph_);
            for(int k = 0; k < graph_->size(); k++)
            {
                my_shortest_paths.calculate(k, empty_path);
                for(int j = 0; j < graph_->size(); j++)
                    if(my_shortest_paths.get_dist(j) != simple_shortest_paths[k][j])
                        return;
            }
        }
        printf("OK\n");
        return;
    }
private:

    Graph<Data, Weight>* graph_;
    std::vector<std::vector<boost::optional<PathLen> > > simple_shortest_paths;
    CalcPath calc_path_;
    CompPath comp_path_;
    void generate_graph()
    {
        graph_->graph_.clear();
        graph_->graph_.resize(1 + rand() % 200);
        for(int i = 0; i < graph_->size(); i++)
            if(i % 2)   graph_->graph_[i] = std::unique_ptr<SetNode<Data, Weight> >(new SetNode<Data, Weight>(Data()));
            else        graph_->graph_[i] = std::unique_ptr<VectorNode<Data, Weight> >(new VectorNode<Data, Weight>(Data()));
        for(int i = 0; i < rand() % 10000; i++)
        {
            graph_->add_edge(rand() % graph_->size(), rand() % graph_->size(), rand() % 1000);
        }
    }
};

#endif // TEST_SHORT_PATHS_H_INCLUDED
