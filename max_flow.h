#ifndef MAX_FLOW_H_INCLUDED
#define MAX_FLOW_H_INCLUDED
#include "shortest_paths_finder.h"

template<typename Weight, typename CompWeight = std::greater<Weight>>
class MaxFlowFinder
{
public:
    explicit MaxFlowFinder(const Graph<Weight>& graph, Weight empty_flow = Weight())
    {
        empty_flow_ = empty_flow;
        for(int i = 0; i < graph.size(); i++)
            graph_.add_node(new VectorNode<Weight>);

        for(int i = 0; i < graph.size(); i++)
        {
            BaseNode<Weight>* curr_node = graph.graph_[i].get();
            for(typename BaseNode<Weight>::Iterator it = curr_node->begin(); it != curr_node->end(); ++it)
            {
                graph_.add_edge(i, **it, (*it).get_weight(), poss_flow_.size());
                graph_.add_edge(**it, i, Weight(), poss_flow_.size() + 1);
                poss_flow_.push_back((*it).get_weight());
                poss_flow_.push_back(empty_flow_);
            }
        }
    }
    Weight calculate_flow(size_t from, size_t to)
    {
        Weight res = empty_flow_;
        while(1)
        {
            ShortestPathFinder<Weight, Weight, CalcWeight, CompWeight> flow_finder(graph_, CalcWeight(poss_flow_));
            flow_finder.calculate(from, std::numeric_limits<Weight>::max());
            if(!flow_finder.get_dist(to)  ||  flow_finder.get_dist(to).get() <= empty_flow_)
                return res;
            Path<Weight> flow = flow_finder.get_path(to);
            Weight curr_flow_value = flow_finder.get_dist(to).get();
            res += curr_flow_value;
            for(int i = 0; i < flow.size(); i++)
            {
                size_t curr_id = (*flow.path_[i]).get_id();
                poss_flow_[curr_id] -= curr_flow_value;
                poss_flow_[get_dual_edge_id(curr_id)] += curr_flow_value;
            }
        }
    }


private:
    Graph<Weight> graph_;
    CompWeight comp_weight_;
    std::vector<Weight> poss_flow_;
    Weight empty_flow_;
    class CalcWeight
    {
    public:
        CalcWeight(const std::vector<Weight> poss_flow): poss_flow_(poss_flow){}
        Weight operator()(const Weight& prev_flow, const Edge<Weight>& new_edge) const
        {
            return comp_weight(prev_flow, poss_flow_[new_edge.get_id()]) ? poss_flow_[new_edge.get_id()] : prev_flow;
        }
    private:
        std::vector<Weight> poss_flow_;
        CompWeight comp_weight;
    };
    size_t get_dual_edge_id(size_t curr_edge_id)
    {
        if(curr_edge_id % 2)
            return (curr_edge_id - 1);
        else
            return (curr_edge_id + 1);
    }
};

#endif // MAX_FLOW_H_INCLUDED
