#ifndef SHORTEST_PATHS_FINDER_H_INCLUDED
#define SHORTEST_PATHS_FINDER_H_INCLUDED
#include "graph.h"
#include <boost/optional.hpp>
#include <vector>


template<typename Weight, typename PathLen = Weight, typename CalcPath = std::plus<PathLen>, typename CompPath = std::less<PathLen> >
class ShortestPathFinder
{
public:
    typedef typename BaseNode<Weight>::Iterator Iterator;
    explicit ShortestPathFinder(const Graph<Weight>& graph, const CalcPath& calc_path = CalcPath()):graph_(graph), calc_path_(calc_path){}
    void calculate(size_t node_num, PathLen empty_path = PathLen())
    {
        info_.clear();
        info_.resize(graph_.size());
        CompState queue_comp(comp_path_, this);
        info_[node_num].dist = empty_path;
        info_[node_num].prev_node = node_num;
        std::set<size_t, CompState> queue(queue_comp);
        queue.insert(node_num);
        while(!queue.empty())
        {
            size_t curr_num = *queue.begin();
            BaseNode<Weight>* curr_node = graph_.graph_[curr_num].get();
            for(Iterator it = curr_node->begin(); it != curr_node->end(); ++it)
            {
                PathLen new_len = calc_path_(info_[curr_num].dist.get(), *it);
                 if(!info_[**it].dist  ||  comp_path_(new_len, info_[**it].dist.get()))
                {
                    if(info_[**it].dist.is_initialized())
                        queue.erase(**it);
                    info_[**it].dist = new_len;
                    info_[**it].prev_node = curr_num;
                    info_[**it].prev_edge = it.copy();
                    queue.insert(**it);
                }
            }
            queue.erase(curr_num);
        }

    }


    boost::optional<PathLen> get_dist(size_t node_num) const
    {
        return info_[node_num].dist;
    }


    Path<Weight> get_path(size_t to) const
    {
        Path<Weight> ret(to);
        while(info_[to].prev_node != to)
        {
            ret.add(info_[to].prev_edge.copy());
            to = info_[to].prev_node;
        }
        ret.reverse_path();
        ret.set_start(to);
        return ret;
    }

private:
    const Graph<Weight>& graph_;
    class State
    {
    public:
        size_t prev_node;
        boost::optional<PathLen> dist;
        Iterator prev_edge;
        State(State&& state):prev_edge(std::move(state.prev_edge)), dist(std::move(state.dist)), prev_node(std::move(state.prev_node)) {}
        State(){}

    };
    std::vector<State> info_;
    CalcPath calc_path_;
    CompPath comp_path_;
    struct CompState
    {
        CompState(const CompPath& comp_path, ShortestPathFinder<Weight, PathLen, CalcPath, CompPath>* curr_finder): curr_finder_(curr_finder), comp_path_(comp_path){}

        bool operator ()(size_t first, size_t second) const
        {
            PathLen& first_obj = curr_finder_->info_[first].dist.get();
            PathLen& second_obj = curr_finder_->info_[second].dist.get();
            if(comp_path_(first_obj, second_obj))
                return true;
            if(comp_path_(second_obj, first_obj))
                return false;
            return(first < second);
        }

        void set_curr_finder(ShortestPathFinder<Weight, PathLen, CalcPath, CompPath>* curr_finder)
        {
            curr_finder_ = curr_finder;
        }
    private:
        CompPath comp_path_;
        ShortestPathFinder<Weight, PathLen, CalcPath, CompPath>* curr_finder_;
    };
};
#endif // SHORTEST_PATHS_FINDER_H_INCLUDED
