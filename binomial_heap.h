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
                    sibling = NULL;
                    parent = NULL;
                    child = NULL;
                }
        };

        node* merge(node *x, node *y)
        {
            if(y == NULL)   return x;
            if(x == NULL)   return y;
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

        std::vector <node*> heap;


        void merge(std::vector <node*> *heap2)
        {
            std::vector <node*> new_heap;
            size_t l = std::max(heap.size(), heap2 -> size());
            new_heap.resize(l + 1, NULL);
            heap2 -> resize(l, NULL);
            heap.resize(l, NULL);
            for(size_t  i = 0 ; i < l; i++)
            {
                if(heap[i] != NULL)
                    if(new_heap[i] == NULL)
                        new_heap[i] = heap[i];
                    else
                    {
                        new_heap[i + 1] = merge(new_heap[i], heap[i]);
                        new_heap[i] = NULL;
                    }
                if((*heap2)[i] != NULL)
                    if(new_heap[i] == NULL)
                        new_heap[i] = (*heap2)[i];
                    else
                    {
                        new_heap[i + 1] = merge(new_heap[i], (*heap2)[i]);
                        new_heap[i] = NULL;
                    }
            }
            heap.swap(new_heap);
            while(heap[heap.size() - 1] == NULL)   heap.resize(heap.size() - 1);
        }


    public:


        binomial_heap()
        {
            size_ = 0;
            heap.clear();
        }


        size_t size()
        {
            return size_;
        }


        size_t count_size(node *x) //this is only for testing
        {
            if(x == NULL)   return 0;
            else            return 1 + count_size(x -> sibling) + count_size(x -> child);
        }


        bool check_sizes() //this is only for testing
        {
            int t = 1, sum_size = 0;
            for(int i = 0; i < heap.size(); i++)
            {
                if(heap[i] != NULL  &&  count_size(heap[i]) != t)   return false;
                sum_size += count_size(heap[i]);
                t = t << 1;
            }
            return(sum_size == size_);
        }

        void merge_and_del(binomial_heap *H)
        {
            if(size_ == 0)
            {
                heap.swap(H -> heap);
                std::swap(size_, H -> size_);
                return;
            }
            if(H == NULL)   return;
            size_ += H -> size_;
            merge(&(H -> heap));
        }


        void insert(T new_key)
        {
            std::vector <node*> new_heap;
            size_++;
            node *now = new node(new_key);
            new_heap.push_back(now);
            merge(&new_heap);
        }


        T extract_min()
        {
            std::vector <node*> new_heap;
            new_heap.clear();
            T ret;
            size_t del_node;
            bool found = false;
            for(size_t i = 0; i < heap.size(); i++)
                if(heap[i] != NULL  &&  (!found  ||  ret > heap[i] -> key))
                {
                    found = true;
                    ret = heap[i] -> key;
                    del_node = i;
                }
            node *i = heap[del_node] -> child;
            while(i != NULL)
            {
                new_heap.push_back(i);
                i = i -> sibling;
                new_heap[new_heap.size() - 1] -> sibling = NULL;
            }
            delete heap[del_node];
            heap[del_node] = NULL;
            std::reverse(new_heap.begin(), new_heap.end());
            merge(&new_heap);
            size_--;
            return ret;
        }


        T get_min() //for testing only
        {
            T ret;
            bool found  = false;
            for(int i = 0; i < heap.size(); i++)
                if(heap[i] != NULL  &&  (!found  ||  ret > heap[i] -> key))
                {
                    found = true;
                    ret = heap[i] -> key;
                }
            return ret;
        }
};

#endif // BINOMIAL_HEAP_H_INCLUDED
