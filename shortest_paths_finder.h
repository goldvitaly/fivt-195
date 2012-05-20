#ifndef SHORTEST_PATHS_FINDER_H_INCLUDED
#define SHORTEST_PATHS_FINDER_H_INCLUDED



#include "graph.h"
#include <boost/optional.hpp>
#include <vector>


template<typename Data, typename Weight, typename PathLen = Weight, typename CalcPath = std::plus<PathLen>, typename CompPath = std::less<PathLen> >
class ShortestPathFinder
{
public:
    explicit ShortestPathFinder(const Graph<Data, Weight>& graph):graph_(graph){}

    void calculate(size_t node_num)
    {
        info_.clear();
        info_.resize(graph_.size());
        CompState queue_comp(comp_path_);
        queue_comp.set_curr_finder(this);
        info_[node_num].dist = PathLen();
        info_[node_num].prev_node = node_num;
        std::set<size_t, CompState> queue(queue_comp);
        queue.insert(node_num);
        while(!queue.empty())
        {
            size_t curr_num = *queue.begin();
            BaseNode<Weight>* curr_node = graph_.graph_[curr_num].get();
            for(typename BaseNode<Weight>::Iterator it = curr_node->begin(); it != curr_node->end(); ++it)
            {
                PathLen new_len = calc_path_(info_[curr_num].dist.get(), (*it).get_weight());
                if(!info_[**it].dist.is_initialized()  ||  comp_path_(new_len, info_[**it].dist.get()))
                {
                    if(info_[**it].dist.is_initialized())
                        queue.erase(**it);
                    info_[**it].dist = new_len;
                    info_[**it].prev_node = curr_num;
                    queue.insert(**it);
                }
            }
            queue.erase(curr_num);
        }
    }


    boost::optional<PathLen> get_dist(size_t node_num)
    {
        return info_[node_num].dist;
    }


    Path<Weight> get_path(size_t to)
    {
        Path<Weight> ret(to);
        while(info_[to].prev_node != to)
        {
            ret.add(info_[to].prev_node);
            to = info_[to].prev_node;
        }
        ret.set_start(to);
        ret.reverse_path();
        return ret;
    }


private:
    const Graph<Data, Weight>& graph_;
    class State
    {
    public:
        size_t prev_node;
        boost::optional<PathLen> dist;
    };
    std::vector<State> info_;
    CalcPath calc_path_;
    CompPath comp_path_;
    struct CompState
    {
        CompState(const CompPath& comp_path): comp_path_(comp_path){}

        bool operator ()(const size_t&   first,const size_t& second) const
        {
            if(comp_path_(curr_finder_->info_[first].dist.get(), curr_finder_->info_[second].dist.get()))
                return true;
            if(comp_path_(curr_finder_->info_[second].dist.get(), curr_finder_->info_[first].dist.get()))
                return false;
            return(first < second);
        }

        void set_curr_finder(ShortestPathFinder<Data, Weight, PathLen, CalcPath, CompPath>* curr_finder)
        {
            curr_finder_ = curr_finder;
        }
    private:
        CompPath comp_path_;
        ShortestPathFinder<Data, Weight, PathLen, CalcPath, CompPath>* curr_finder_;
    };
};
#endif // SHORTEST_PATHS_FINDER_H_INCLUDED
