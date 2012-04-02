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
    virtual void delete_edge(int to) = 0;
    virtual size_t number_of_edges() = 0;

    class BaseIterator
    {
    public:
        virtual bool operator == (const BaseIterator& it) const = 0;
        virtual bool operator != (const BaseIterator& it) const = 0;
        virtual int operator * () = 0;
        virtual BaseIterator* operator ++ () = 0;
        ~BaseIterator(){};
    };

    virtual BaseIterator* begin() = 0;
    virtual BaseIterator* end() = 0;
    virtual ~BaseNode(){};
};


template<class T>
class SetNode: public BaseNode
{
public:
    SetNode(T value)
    {
        value_ = value;
        edges_.clear();
    }

    void add_edge(int to)
    {
        edges_.insert(to);
    }

    void delete_edge(int to)
    {
        edges_.erase(to);
    }

    size_t number_of_edges()
    {
        return edges_.size();
    }

    class SetIterator:public BaseIterator
    {
    public:

        SetIterator(std::multiset<int>::iterator it = NULL)
        {
            iterator_ = it;
        }
        bool operator ==(const BaseIterator& it) const
        {
            SetIterator set_it = dynamic_cast<const SetIterator&>(it);
            return(set_it.iterator_ == iterator_);
        }

        bool operator !=(const BaseIterator& it) const
        {
            return(!(*this == it));
        }

        int operator * ()
        {
            return (*iterator_);
        }

        SetIterator* operator ++()
        {
            iterator_++;
            return this;
        }

        ~SetIterator()
        {
        }
    private:
        std::multiset<int>::iterator iterator_;
    };


    BaseIterator* begin()
    {
        return (new SetIterator(edges_.begin()));
    }

    BaseIterator* end()
    {
        return (new SetIterator(edges_.end()));
    }

    ~SetNode()
    {
        edges_.clear();
    }
private:
    std::multiset<int> edges_;
    T value_;
};


template<class T>
class VectorNode: public BaseNode
{
public:
    VectorNode(T value = 0)
    {
        value_ = value;
        edges_.clear();
    }

    void add_edge(int to)
    {
        edges_.push_back(to);
    }

    void delete_edge(int to)
    {
        std::vector<int>::iterator it = find(edges_.begin(), edges_.end(), to);
        if(it != edges_.end())
            edges_.erase(it);
    }

    size_t number_of_edges()
    {
        return edges_.size();
    }

    class VectorIterator:public BaseIterator
    {
    public:

        VectorIterator(std::vector<int>::iterator it = NULL)
        {
            iterator_ = it;
        }

        bool operator ==(const BaseIterator& it) const
        {
            VectorIterator vector_it = dynamic_cast<const VectorIterator&>(it);
            return(vector_it.iterator_ == iterator_);
        }

        bool operator !=(const BaseIterator& it) const
        {
            return(!(*this == it));
        }

        int operator * ()
        {
            return *iterator_;
        }

        VectorIterator* operator ++()
        {
            iterator_++;
            return this;
        }

        ~VectorIterator()
        {
        }
    private:
        std::vector<int>::iterator iterator_;
    };


    BaseIterator* begin()
    {
        return (new VectorIterator(edges_.begin()));
    }

    BaseIterator* end()
    {
        return (new VectorIterator(edges_.end()));
    }

    ~VectorNode()
    {
        edges_.clear();
    }
private:
    std::vector<int> edges_;
    T value_;
};

#endif // BASE_GRAPH_H_INCLUDED
