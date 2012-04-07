#ifndef BASE_GRAPH_H_INCLUDED
#define BASE_GRAPH_H_INCLUDED
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <set>


class BaseNode
{
public:

    virtual void add_edge(size_t to) = 0;
    virtual void delete_edge(size_t to) = 0;
    virtual size_t number_of_edges() const = 0;

    class BaseIterator
    {
    public:
        virtual bool operator == (const BaseIterator& it) const = 0;
        virtual bool operator != (const BaseIterator& it) const = 0;
        virtual size_t operator * () const = 0;
        virtual BaseIterator& operator ++ () = 0;
        ~BaseIterator(){};
    };


    class Iterator
    {
    public:
        explicit Iterator(BaseIterator* it)
        {
            iterator_ = std::unique_ptr<BaseIterator>(it);
        }
        bool operator == (const Iterator& it)
        {
            return(*iterator_ == *(it.iterator_));
        }

        bool operator != (const Iterator& it)
        {
            return(*iterator_ != *(it.iterator_));
        }

        int operator * () const
        {
            return **iterator_;
        }

        Iterator& operator ++()
        {
            ++(*iterator_);
        }
    private:
        std::unique_ptr<BaseIterator> iterator_;
    };

    virtual Iterator begin() = 0;
    virtual Iterator end() = 0;
    ~BaseNode(){};
};


template<class T>
class SetNode: public BaseNode
{
public:
    explicit SetNode(const T& value)
    {
        value_ = value;
        edges_.clear();
    }

    void add_edge(size_t to)
    {
        edges_.insert(to);
    }

    void delete_edge(size_t to)
    {
        edges_.erase(to);
    }

    size_t number_of_edges() const
    {
        return edges_.size();
    }

    class SetIterator:public BaseIterator
    {
    public:

        SetIterator(std::multiset<size_t>::iterator it = NULL)
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

        size_t operator * () const
        {
            return (*iterator_);
        }

        SetIterator& operator ++()
        {
            iterator_++;
            return *this;
        }

        ~SetIterator()
        {
        }
    private:
        std::multiset<size_t>::iterator iterator_;
    };


    Iterator begin()
    {
        return Iterator(new SetIterator(edges_.begin()));
    }

    Iterator end()
    {
        return Iterator(new SetIterator(edges_.end()));
    }

    ~SetNode()
    {
        edges_.clear();
    }
private:
    std::multiset<size_t> edges_;
    T value_;
};


template<class T>
class VectorNode: public BaseNode
{
public:
    explicit VectorNode(const T& value = 0)
    {
        value_ = value;
        edges_.clear();
    }

    void add_edge(size_t to)
    {
        edges_.push_back(to);
    }

    void delete_edge(size_t to)
    {
        std::vector<size_t>::iterator it = find(edges_.begin(), edges_.end(), to);
        if(it != edges_.end())
            edges_.erase(it);
    }

    size_t number_of_edges() const
    {
        return edges_.size();
    }

    class VectorIterator:public BaseIterator
    {
    public:

        explicit VectorIterator(std::vector<size_t>::iterator it = NULL)
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

        size_t operator * () const
        {
            return *iterator_;
        }

        VectorIterator& operator ++()
        {
            iterator_++;
            return *this;
        }

        ~VectorIterator()
        {
        }
    private:
        std::vector<size_t>::iterator iterator_;
    };


    Iterator begin()
    {
        return Iterator(new VectorIterator(edges_.begin()));
    }

    Iterator end()
    {
        return Iterator(new VectorIterator(edges_.end()));
    }

    ~VectorNode()
    {
        edges_.clear();
    }
private:
    std::vector<size_t> edges_;
    T value_;
};

#endif // BASE_GRAPH_H_INCLUDED
