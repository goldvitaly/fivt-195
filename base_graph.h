#ifndef BASE_GRAPH_H_INCLUDED
#define BASE_GRAPH_H_INCLUDED
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <set>

template <class Weight>
class Edge
{
    public:
        explicit Edge(const size_t& to, const Weight& weight)
        {
            weight_ = weight;
            direction_ = to;
        }

        void set_weight(Weight new_weight)
        {
            weight_ = new_weight;
        }


        Weight get_weight() const
        {
            return weight_;
        }

        size_t operator * () const
        {
            return direction_;
        }

        bool operator == (const Edge<Weight>& edge) const //This operator for STL's features
        {
            return(direction_ == *edge  &&  weight_ == edge.get_weight());
        }

        bool operator < (const Edge<Weight>& edge) const //This operator for STL's features
        {
            if(direction_ == *edge)
                return(weight_ < edge.get_weight());
            return(direction_ < *edge);
        }
    private:
        size_t direction_;
        Weight weight_;
};


template <class Weight>
class Path
{
public:
    explicit Path(size_t direction): direction_(direction) {}

    void add(size_t new_node)
    {
        path_.push_back(new_node);
    }

    void set_start(size_t start)
    {
        start_ = start;
    }


    void print()
    {
        for(int i = 0; i < path_.size(); i++)
            std::cout << path_[i] << ' ';
        std::cout << direction_ << std::endl;
    }

    void reverse_path()
    {
        reverse(path_.begin(), path_.end());
    }
private:
    std::vector<size_t> path_;
    size_t direction_, start_;
};


template <class Weight>
class BaseNode
{
public:
    virtual void add_edge(Edge<Weight> edge) = 0;
    virtual void delete_edge(Edge<Weight> edge) = 0;
    virtual size_t number_of_edges() const = 0;

    class BaseIterator
    {
    public:
        virtual bool operator == (const BaseIterator& it) const = 0;
        virtual bool operator != (const BaseIterator& it) const = 0;
        virtual Edge<Weight> operator * () const = 0;
        virtual BaseIterator& operator ++ () = 0;
        virtual ~BaseIterator(){};
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

        Edge<Weight> operator * () const
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
    virtual ~BaseNode(){};
};


template<class Data, class Weight>
class SetNode: public BaseNode<Weight>
{
public:
    typedef typename BaseNode<Weight>::BaseIterator BaseIterator;
    typedef typename BaseNode<Weight>::Iterator Iterator;


    explicit SetNode(const Data& value)
    {
        value_ = value;
        edges_.clear();
    }

    void add_edge(Edge<Weight> edge)
    {
        edges_.insert(edge);
    }

    void delete_edge(Edge<Weight> edge)
    {
        edges_.erase(edge);
    }

    size_t number_of_edges() const
    {
        return edges_.size();
    }

    class SetIterator:public BaseIterator
    {
    public:

        SetIterator(typename std::multiset<Edge<Weight> >::iterator it = NULL)
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


        Edge<Weight> operator * () const
        {
            return (*iterator_);
        }

        SetIterator& operator ++()
        {
            iterator_++;
            return *this;
        }

        virtual ~SetIterator() {}
    private:
        typename std::multiset<Edge<Weight> >::iterator iterator_;
    };


    Iterator begin()
    {
        return Iterator(new SetIterator(edges_.begin()));
    }

    Iterator end()
    {
        return Iterator(new SetIterator(edges_.end()));
    }

    ~SetNode()  {}
private:
    std::multiset< Edge<Weight> > edges_;
    Data value_;
};


template<class Data, class Weight>
class VectorNode: public BaseNode<Weight>
{
public:
    typedef typename BaseNode<Weight>::BaseIterator BaseIterator;
    typedef typename BaseNode<Weight>::Iterator Iterator;


    explicit VectorNode(const Data& value = 0)
    {
        value_ = value;
        edges_.clear();
    }

    void add_edge(Edge<Weight> edge)
    {
        edges_.push_back(edge);
    }

    void delete_edge(Edge<Weight> edge)
    {
        typename std::vector<Edge<Weight> >::iterator it = find(edges_.begin(), edges_.end(), edge);
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

        explicit VectorIterator(typename std::vector<Edge<Weight> >::iterator it = NULL)
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

        Edge<Weight> operator * () const
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
        typename std::vector<Edge<Weight> >::iterator iterator_;
    };


    Iterator begin()
    {
        return Iterator(new VectorIterator(edges_.begin()));
    }

    Iterator end()
    {
        return Iterator(new VectorIterator(edges_.end()));
    }

    ~VectorNode() {}
private:
    std::vector<Edge<Weight> > edges_;
    Data value_;
};

#endif // BASE_GRAPH_H_INCLUDED
