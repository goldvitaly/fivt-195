#ifndef INTERVAL_TREE_H_INCLUDED
#define INTERVAL_TREE_H_INCLUDED
#include <limits>
#include "interval_tree_kernel.h"

template <class T>
struct default_RMQ_merger
{
    T operator()(T x, T y)
    {
        return std::min(x, y);
    }
};

template <class T>
struct RMSQ_mod_type
{
    bool change;
    T change_to, modify;
    RMSQ_mod_type()
    {
        change = false;
        change_to = 0;
        modify = 0;
    }
};

template <class T>
struct RMSQ_merge_mod_func
{
    RMSQ_mod_type<T> operator()(RMSQ_mod_type<T> added, RMSQ_mod_type<T> prev)
    {
        if(added.change)
        {
            prev.modify = 0;
            prev.change = true;
            prev.change_to = added.change_to;
        }
        else
        {
            if(prev.change) prev.change_to += added.modify;
            else            prev.modify += added.modify;
        }
        return prev;
    }
};

template <class T, class RMQ_merger = default_RMQ_merger<T> >
class RMQ
{
private:

    struct RMQ_mod_func
    {
        T operator ()(T key_, RMSQ_mod_type<T> mod, size_t range_size)
        {
            if(mod.change)  key_ = mod.change_to;
            else            key_ += mod.modify;
            return key_;
        }
    };



public:

    interval_tree<T, RMSQ_mod_type<T>, RMQ_merger, RMQ_mod_func, RMSQ_merge_mod_func<T> > *tree;


    template<class It>
    RMQ(It begin, It end)
    {
        tree = new interval_tree<T, RMSQ_mod_type<T>, RMQ_merger, RMQ_mod_func, RMSQ_merge_mod_func<T> >(begin, end, std::numeric_limits<T>::max());
    }

    T get_min_from_range(size_t left, size_t right)
    {
        return tree->get_from_range(left, right);
    }

    void change_range(size_t left, size_t right, T change_to)
    {
        RMSQ_mod_type<T> delta;
        delta.change = true;
        delta.change_to = change_to;
        tree->mod_range(left, right, delta);
    }

    void mod_range(size_t left, size_t right, T modify)
    {
        RMSQ_mod_type<T> delta;
        delta.modify = modify;
        tree->mod_range(left, right, delta);
    }
};


template <class T, class RSQ_merger = std::plus<T> >
class RSQ
{
private:

    struct RSQ_mod_func
    {
        T operator ()(T key_, RMSQ_mod_type<T> mod, size_t range_size)
        {
            if(mod.change)  key_ = mod.change_to * range_size;
            else            key_ += mod.modify * range_size;
            return key_;
        }
    };

public:

    interval_tree<T, RMSQ_mod_type<T>, RSQ_merger, RSQ_mod_func, RMSQ_merge_mod_func<T> > *tree;


    template<class It>
    RSQ(It begin, It end)
    {
        tree = new interval_tree<T, RMSQ_mod_type<T>, RSQ_merger, RSQ_mod_func, RMSQ_merge_mod_func<T> >(begin, end, 0);
    }

    T get_sum_from_range(size_t left, size_t right)
    {
        return tree->get_from_range(left, right);
    }

    void change_range(size_t left, size_t right, T change_to)
    {
        RMSQ_mod_type<T> delta;
        delta.change = true;
        delta.change_to = change_to;
        tree->mod_range(left, right, delta);
    }

    void mod_range(size_t left, size_t right, T modify)
    {
        RMSQ_mod_type<T> delta;
        delta.modify = modify;
        tree->mod_range(left, right, delta);
    }
    void print_last_level()
    {
        tree->print_last_level();
    }
};
#endif // INTERVAL_TREE_H_INCLUDED
