#ifndef INTERVAL_TREE_H_INCLUDED
#define INTERVAL_TREE_H_INCLUDED
#include <iostream>
#include <cstdio>
#include <vector>




template <class element, class mod_type, class merge_func, class mod_func, class merge_mod_func>
class interval_tree
{
    private:
        class interval
        {
            public:
            size_t left, right;
            interval(size_t l = 0, size_t r = 0)
            {
                left = l;
                right = r;
            }
            size_t length()
            {
                return (right - left  + 1);
            }
        };


        interval intersection(const interval &a, const interval &b)
        {
            return(interval(std::max(a.left, b.left), std::min(a.right, b.right)));
        }

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
            tree_element(element neutral)
            {
                key = neutral;
                is_modified = false;
            }
        };

        std::vector <tree_element> tree_;

        size_t base_;

        bool inside(const interval &inner, const interval &outer)
        {
            return(inner.left >= outer.left  &&  inner.right <= outer.right);
        }


        bool outside(const interval &inner, const interval &outer)
        {
            return(inner.left > outer.right  ||  inner.right < outer.left);
        }


        void mod_childs(size_t now)
        {
            if(tree_[now].is_modified)
            {
                tree_[now].is_modified = false;
                if(now < base_)
                    for(size_t i = now * 2; i <= now * 2 + 1; i++)
                    {
                        tree_[i].key = modifier_.modify(tree_[i].key, tree_[now].delta, tree_[i].range.length());
                        if(tree_[i].is_modified)
                            tree_[i].delta = mod_merger_.merge(tree_[now].delta, tree_[i].delta);
                        else
                        {
                            tree_[i].delta = tree_[now].delta;
                            tree_[i].is_modified = true;
                        }
                    }
            }
        }
        const element get_from_range(size_t now, interval range)
        {
            mod_childs(now);
            if(inside(tree_[now].range, range)) return tree_[now].key;
            if(outside(tree_[now].range, range)) return neutral_;
            return (merger_.merge(get_from_range(now * 2, range), get_from_range(now * 2 + 1, range)));
        }


        void mod_range(size_t now, interval range, mod_type update)
        {
            if(inside(tree_[now].range, range))
            {
                tree_[now].key = modifier_.modify(tree_[now].key, update, tree_[now].range.length());
                if(!tree_[now].is_modified)
                {
                    tree_[now].is_modified = true;
                    tree_[now].delta = update;
                }
                else    tree_[now].delta = mod_merger_.merge(tree_[now].delta, update);
                return;
            }
            if(outside(tree_[now].range, range)) return;
            mod_childs(now);
            mod_range(now * 2, range, update);
            mod_range(now * 2 + 1, range, update);
            tree_[now].key = merger_.merge(tree_[now * 2].key, tree_[now * 2 + 1].key);
        }
    public:
        explicit interval_tree(const std::vector <element> &items, element neutral)
        {
            neutral_ = neutral;
            base_ = 1;
            while(base_ < items.size()) base_ = base_ << 1;
            tree_.resize(base_ * 2, tree_element(neutral_));
            for(size_t i = 0; i < items.size(); i++)
                tree_[base_ + i].key = items[i];
            for(size_t i = 0; i < base_; i++)
            {
                tree_[base_ + i].range.left = i + 1;
                tree_[base_ + i].range.right = i + 1;
            }
            for(size_t i = base_ - 1; i > 0; i--)
            {
                tree_[i].key = merger_.merge(tree_[i * 2].key, tree_[i * 2 + 1].key);
                tree_[i].range.left = tree_[i * 2].range.left;
                tree_[i].range.right = tree_[i * 2 + 1].range.right;
            }
        }

        const element get_from_range(size_t l, size_t r)
        {
            return get_from_range(1, interval(l, r));
        }


        void mod_range(size_t l, size_t r, mod_type update)
        {
            mod_range(1, interval(l, r), update);
        }

        void print_bottom() //for testing inly
        {
            for(int i = 0; i < base_; i++)
                std::cout << tree_[base_ + i].key << ' ';
        }
};


#endif // INTERVAL_TREE_H_INCLUDED
