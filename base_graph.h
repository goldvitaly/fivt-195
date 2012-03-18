#ifndef BASE_GRAPH_H_INCLUDED
#define BASE_GRAPH_H_INCLUDED
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <set>


class BaseNode
{
public:

    virtual void add_edge(int to) = 0;
    virtual void add_inc_edge(int to) = 0;
    virtual void delete_edge(int to) = 0;
    virtual void delete_inc_edge(int to) = 0;
    virtual void inverse_edges() = 0;
    virtual size_t number_of_edges() = 0;
    virtual std::vector<int> get_neighbours() = 0;
    bool flag_for_dfs_;
    int time_in, time_out;
};


template<class T>
class SetNode: public BaseNode
{
public:
    bool flag_for_dfs_;
    int time_in, time_out;
    SetNode(T value)
    {
        value_ = value;
        time_in = 0;
        time_out = 0;
        flag_for_dfs_ = 0;
    }
    void add_edge(int to)
    {
        edges_.insert(to);
    }

    void add_inc_edge(int from)
    {
        inc_edges_.insert(from);
    }

    void delete_edge(int to)
    {
        edges_.erase(to);
    }

    void delete_inc_edge(int from)
    {
        inc_edges_.erase(from);
    }

    void inverse_edges()
    {
        edges_.swap(inc_edges_);
    }

    size_t number_of_edges()
    {
        return edges_.size();
    }

    std::vector<int> get_neighbours()
    {
        std::vector<int> neighbours;
        for(std::multiset<int>::iterator it = edges_.begin(); it != edges_.end(); it++)
            neighbours.push_back(*it);
        return neighbours;
    }


private:
    T value_;
    std::multiset <int> edges_, inc_edges_;
};
#endif // BASE_GRAPH_H_INCLUDED
