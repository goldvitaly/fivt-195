#ifndef INCIDENTS_H_INCLUDED
#define INCIDENTS_H_INCLUDED

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>

template<typename TWeight>
struct Edge
{
public:
    TWeight weight;
    size_t to;

    Edge(size_t _to, const TWeight& _weight = TWeight()): to(_to), weight(_weight) {}

    bool operator < (const Edge<TWeight>& other) const
    {
        return (to < other.to) || (to == other.to && weight < other.weight);
    }

    bool operator == (const Edge<TWeight>& other) const
    {
        return (to == other.to) && (weight == other.weight);
    }
};


template<typename TWeight>
class IncidenceCallBack 
{
private:
    IncidenceCallBack& operator = (const IncidenceCallBack<TWeight>& cpy);
    IncidenceCallBack (const IncidenceCallBack<TWeight>& other);
public:
    IncidenceCallBack() {}
    virtual void operator() (const Edge<TWeight>& edge) = 0;
    virtual ~IncidenceCallBack() {};
};


template<typename TWeight>
class Vertex
{
protected:
    virtual void deletingError()
    {
        std::cerr << "Deleting error" << std::endl;
    }
public:
    virtual void addNeighbour(const Edge<TWeight> & edge) = 0;
    virtual void delNeighbour(const Edge<TWeight> & edge) = 0;
    virtual void delNeighbours(size_t v) = 0;
    virtual bool isConnect(const Edge<TWeight> & edge) const = 0;
    virtual bool isConnect(size_t v) const = 0;
    virtual size_t degree() const = 0;
    virtual void clear() = 0;
    virtual void incedents(IncidenceCallBack<TWeight>& cb) const = 0;
    virtual ~Vertex () {};
};

template<typename TWeight>
class IncidenceSet : public Vertex<TWeight>
{
private:
    std::set< Edge<TWeight> > adj;
public:

    void addNeighbour(const Edge<TWeight> & newEdge) override
    {
        incidents.insert(newEdge);
    }

    void delNeighbour(const Edge<TWeight> & deletingEdge) override
    {
        std::set< Edge<TWeight> >:: iterator it = adj.find(deletingEdge);
        if (it == adj.end())
            deletingError();
        else
            adj.erase(it);
    }
    void delNeighbours(size_t v) const override
    {
        if (adj.size() == 0)
        return;
        Edge e1(v);
        std::set< Edge<TWeight> >::iterator it1 = adj.upper_bound(e1);
        if (it1 == adj.end())
        {
            it2 = adj.end();
            --it2;
            if (it2->to == v)
            {
                adj.erase(it2);
                delNeighbours(v);
            }
        }
        else
        {
            if (it1->to == v)
            {
                adj.erase(it1);
                delNeighbours(v);
            }
        }
    }
    bool isConnect(const Edge<TWeight> & findingEdge) const override
    {
        return (adj.find(findingEdge) != adj.end());
    }
    bool isConnect(size_t v) const override
    {
        if (adj.size() == 0)
        return false;
        Edge edge(v);
        std::set< Edge<TWeight> >::iterator it1 = adj.upper_bound(edge);
        if (it1 == adj.end())
        {
            it2 = adj.end();
            --it2;
            return (it2->to == v);
        }
        else
        {
            if (it1->to == v)
                return true;
            std::set<Edge>::iterator it2 = it1;
            ++it2;
            if (it2 != adj.end() && it2->to == v)
                return true;
            return false;
        }
    }
    size_t degree() const override
    {
        return adj.size();
    }
    void clear() override
    {
        adj.clear();
    }
    void incedents(IncidenceCallBack& cb) const override
    {
        for (auto it = adj.begin(); it != adj.end(); ++it)
    	    cb(*it);
    }
};

template<typename TWeight>
class IncidenceList : public Vertex<TWeight>
{
private:
    std::vector< Edge<TWeight> > adj;
public:
    void addNeighbour(const Edge<TWeight> & newEdge) override
    {
        adj.push_back(newEdge);
    }
    void delNeighbour(const Edge<TWeight> & deletingEdge) override
    {
        bool isDeleted = false;
        for (size_t pos = 0; pos < adj.size(); ++pos)
            if (adj[pos] == deletingEdge)
            {
                std::swap(adj[pos], adj[adj.size() - 1]);
                adj.pop_back();
                isDeleted = true;
                break;
            }
        if (!isDeleted)
        {
            deletingError();
        }
    }
    void delNeighbours(size_t deletingVertex) override
    {
        size_t qDeleting = std::count_if (adj.begin(), adj.end(), [deletingVertex] (Edge<TWeight> curEdge)
        {
            return (deletingVertex == curEdge.to);
        });
        if (!qDeleting)
        {
            deletingError();
        }
        else
        {
            size_t last = adj.size() - 1;
            for (size_t pos = 0; pos < adj.size() - qDeleting; ++pos)
            {
                if (adj[pos].to == deletingVertex)
                {
                    while (adj[last].to == deletingVertex)
                        --last;
                    std::swap(adj[pos], adj[last]);
                }
            }
            adj.erase(adj.end() - qDeleting, adj.end());
        }
    }
    bool isConnect(const Edge<TWeight> & findingEdge) const override
    {
        auto it =
            std::find_if(adj.begin(), adj.end(), [findingEdge] (Edge<TWeight> curEdge)
	    {
	        return (findingEdge == curEdge);
    	});
        return (it != adj.end());
    }
    bool isConnect(size_t findingVertex) const override
    {
        auto it = std::find_if(adj.begin(), adj.end(), [findingVertex] (Edge<TWeight> curEdge)
 	   	{
    	    return (findingVertex == curEdge.to);
    	});
        return (it != adj.end());
    }
    size_t degree() const override
    {
        return adj.size();
    }
    void clear() override
    {
        adj.clear();
    }
    void incedents(IncidenceCallBack<TWeight>& cb) const override
    {
        for (auto it = adj.begin(); it != adj.end(); ++it)
        	cb(*it);
    }
};

#endif