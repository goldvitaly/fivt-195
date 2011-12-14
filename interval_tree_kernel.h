#ifndef INTERVAL_TREE_KERNEL_H_INCLUDED
#define INTERVAL_TREE_KERNEL_H_INCLUDED

#include <iostream>
#include <cstdio>
#include <vector>
template <class element, //key type for tree nodes
class mod_type,   //type which contains only modification parameters
class merge_func, //functor calculates parent from childs
class mod_func,   //functor which modificates node by mod_type
class merge_mod_func> //functor merges 2 modifications for one node in one mod_type
class interval_tree
{
private:
    class interval
    {
        public:
        size_t left, right;
        interval()
        {
            left = 0;
            right = 0;
        }
        interval(size_t l, size_t r)
        {
            left = l;
            right = r;
            right = r;
        }
        size_t length() const
        {
            return (right - left  + 1);
        }

        bool inside_of(const interval &a) const
        {
            return(a.left <= left  &&  a.right >= right);
        }


        bool outside_of(const interval &a) const
        {
            return(right < a.left  ||  left > a.right);
        }
    };

    element neutral_;
    merge_func merger_;
    mod_func modifier_;
    merge_mod_func mod_merger_;

    struct tree_element
    {
        element key;
        interval range;
        mod_type delta;
        bool is_modified;
        explicit tree_element(const element &neutral)
        {
            key = neutral;
            is_modified = false;
        }
    };

    std::vector <tree_element> tree_;

    size_t last_level_size_;

    void mod_child(size_t parent_number, size_t child_number)
    {
        tree_[child_number].key = modifier_(tree_[child_number].key, tree_[parent_number].delta, tree_[child_number].range.length());
        if(tree_[child_number].is_modified)
            tree_[child_number].delta = mod_merger_(tree_[parent_number].delta, tree_[child_number].delta);
        else
        {
            tree_[child_number].delta = tree_[parent_number].delta;
            tree_[child_number].is_modified = true;
        }
    }


    void mod_childs(size_t node_number)
    {
        if(tree_[node_number].is_modified)
        {
            tree_[node_number].is_modified = false;
            if(node_number < last_level_size_)
            {
                mod_child(node_number, node_number * 2);
                mod_child(node_number, node_number * 2 + 1);
            }
        }
    }


    element get_from_range(size_t node_number, const interval &range)
    {
        mod_childs(node_number);
        if(tree_[node_number].range.inside_of(range)) return tree_[node_number].key;
        if(tree_[node_number].range.outside_of(range)) return neutral_;
        return (merger_(get_from_range(node_number * 2, range), get_from_range(node_number * 2 + 1, range)));
    }


    void mod_range(size_t node_number, interval range, const mod_type &update)
    {
        mod_childs(node_number);
        if(tree_[node_number].range.inside_of(range))
        {
            tree_[node_number].key = modifier_(tree_[node_number].key, update, tree_[node_number].range.length());
            if(!tree_[node_number].is_modified)
            {
                tree_[node_number].is_modified = true;
                tree_[node_number].delta = update;
            }
            else    tree_[node_number].delta = mod_merger_(tree_[node_number].delta, update);
            return;
        }
        if(tree_[node_number].range.outside_of(range)) return;
        mod_range(node_number * 2, range, update);
        mod_range(node_number * 2 + 1, range, update);
        tree_[node_number].key = merger_(tree_[node_number * 2].key, tree_[node_number * 2 + 1].key);
    }
public:
    template<class It>
    interval_tree(It begin, It end, const element &neutral)
    {
        neutral_ = neutral;
        last_level_size_ = 1;
        size_t number_of_elements = std::distance(begin, end);
        while(last_level_size_ < number_of_elements) last_level_size_ = last_level_size_ << 1;
        tree_.resize(last_level_size_ * 2, tree_element(neutral_));
        for(size_t i = 0; i < number_of_elements; i++)
        {
            tree_[last_level_size_ + i].key = *(begin + i);
        }
        for(size_t i = 0; i < last_level_size_; i++)
        {
            tree_[last_level_size_ + i].range.left = i + 1;
            tree_[last_level_size_ + i].range.right = i + 1;
        }
        for(size_t i = last_level_size_ - 1; i > 0; i--)
        {
            tree_[i].key = merger_(tree_[i * 2].key, tree_[i * 2 + 1].key);
            tree_[i].range.left = tree_[i * 2].range.left;
            tree_[i].range.right = tree_[i * 2 + 1].range.right;
        }
    }

    element get_from_range(size_t left, size_t right)
    {
        return get_from_range(1, interval(left, right));
    }


    void mod_range(size_t left, size_t right, const mod_type &update)
    {
        mod_range(1, interval(left, right), update);
    }

    void print_last_level()
    {
        for(int i = 0; i < last_level_size_; i++)
            std::cout << tree_[last_level_size_ + i].key << ' ';
        std::cout << std::endl;
    }
};

#endif // INTERVAL_TREE_KERNEL_H_INCLUDED
