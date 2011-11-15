#ifndef BINOMIAL_HEAP_H_INCLUDED
#define BINOMIAL_HEAP_H_INCLUDED

#include <vector>
#include <algorithm>


template <class T>
class binomial_heap
{
    private:

        size_t size_;
        struct node
        {
            public:
                node *sibling, *parent, *child;
                T key;

                node(T key_)
                {
                    key = key_;
                    sibling = 0;
                    parent = 0;
                    child = 0;
                }
        };
        std::vector <node*> heap_;

        node* merge(node *x, node *y)
        {
            if(y == 0)   return x;
            if(x == 0)   return y;
            if(x -> key <= y -> key)
            {
                y -> sibling = x -> child;
                x -> child = y;
                y -> parent = x;
                return x;
            }
            else
            {
                x -> sibling = y -> child;
                y -> child = x;
                x -> parent = y;
                return y;
            }
        }




        void merge(std::vector <node*> *heap2)
        {
            std::vector <node*> new_heap;
            size_t l = std::max(heap_.size(), heap2 -> size());
            new_heap.resize(l + 1, 0);
            heap2 -> resize(l, 0);
            heap_.resize(l, 0);
            for(size_t  i = 0 ; i < l; i++)
            {
                node *for_check;
                for_check = heap_[i];
                for(int j = 0; j < 2; j++)
                {
                    if(j)
                        for_check = (*heap2)[i];
                    if(for_check != 0)
                        if(new_heap[i] == 0)
                            new_heap[i] = for_check;
                        else
                        {
                            new_heap[i + 1] = merge(new_heap[i], for_check);
                            new_heap[i] = 0;
                        }
                }
            }
            heap_.swap(new_heap);
            while(heap_[heap_.size() - 1] == 0)   heap_.resize(heap_.size() - 1);
        }


    public:


        binomial_heap()
        {
            size_ = 0;
            heap_.clear();
        }


        const size_t size()
        {
            return size_;
        }


        size_t count_size(node *x) //this is only for testing
        {
            if(x == 0)   return 0;
            else            return 1 + count_size(x -> sibling) + count_size(x -> child);
        }


        bool check_sizes() //this is only for testing
        {
            int t = 1, sum_size = 0;
            for(int i = 0; i < heap_.size(); i++)
            {
                if(heap_[i] != 0  &&  count_size(heap_[i]) != t)   return false;
                sum_size += count_size(heap_[i]);
                t = t << 1;
            }
            return(sum_size == size_);
        }

        void merge_and_del(binomial_heap *H)
        {
            if(size_ == 0)
            {
                heap_.swap(H -> heap_);
                std::swap(size_, H -> size_);
                return;
            }
            if(H == 0)   return;
            size_ += H -> size_;
            merge(&(H -> heap_));
        }


        void insert(T new_key)
        {
            std::vector <node*> new_heap;
            size_++;
            node *now = new node(new_key);
            new_heap.push_back(now);
            merge(&new_heap);
        }


        const T extract_min()
        {
            std::vector <node*> new_heap;
            new_heap.clear();
            T ret;
            size_t del_node;
            bool found = false;
            for(size_t i = 0; i < heap_.size(); i++)
                if(heap_[i] != 0  &&  (!found  ||  ret > heap_[i] -> key))
                {
                    found = true;
                    ret = heap_[i] -> key;
                    del_node = i;
                }
            node *i = heap_[del_node] -> child;
            while(i != 0)
            {
                new_heap.push_back(i);
                i = i -> sibling;
                new_heap[new_heap.size() - 1] -> sibling = 0;
            }
            delete heap_[del_node];
            heap_[del_node] = 0;
            std::reverse(new_heap.begin(), new_heap.end());
            merge(&new_heap);
            size_--;
            return ret;
        }


        const T get_min()
        {
            T ret;
            bool found  = false;
            for(int i = 0; i < heap_.size(); i++)
                if(heap_[i] != 0  &&  (!found  ||  ret > heap_[i] -> key))
                {
                    found = true;
                    ret = heap_[i] -> key;
                }
            return ret;
        }
};

#endif // BINOMIAL_HEAP_H_INCLUDED
