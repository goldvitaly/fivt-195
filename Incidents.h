#ifndef INCIDENTS_H_INCLUDED
#define INCIDENTS_H_INCLUDED

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>

class IncidenceCallBack //further may referred ICB
{
private:
    //ICB& operator = (const ICB& cpy);
    //ICB (const ICB& other);
public:
    IncidenceCallBack() {};
    virtual void operator() (size_t v) = 0;
    virtual ~IncidenceCallBack() {};
};

class ICBprint : public IncidenceCallBack
{
private:
    std::ostream* out;
public:
    ICBprint (std::ostream & _out)
    {
        out = &_out;
    };
    void operator()(size_t v)
    {
        *out << v << std::endl;
    }
};

class Vertex
{
protected:
    virtual void deletingError()
    {
        std::cerr << "Deleting error" << std::endl;
    }
public:
    virtual void addNeighbour(size_t v) = 0;
    virtual void delNeighbour(size_t v) = 0;
    virtual bool isConnect(size_t v) const = 0;
    virtual size_t degree() const = 0;
    virtual void clear() = 0;
    virtual void incedents(IncidenceCallBack& cb) const = 0;
    virtual ~Vertex () {};
};

class IncidenceVectorBool : public Vertex
{
private:
    std::vector<bool> adj;
    size_t deg;
public:
    IncidenceVectorBool () : deg(0) {};
    void addNeighbour(size_t v)
    {
        if (v >= adj.size())
            adj.resize(v + 1);
        if (!adj[v])
        {
            adj[v] = true;
            deg++;
        }
    }
    void delNeighbour(size_t v)
    {
        if (v >= adj.size() || adj[v])
        {
            deletingError();
            return;
        }
        if (adj[v])
        {
            deg--;
            adj[v] = false;
        }
    }
    bool isConnect(size_t v) const
    {
        if (v >= adj.size())
            return false;
        return adj[v];
    }
    size_t degree() const
    {
        return deg;
    }
    void clear()
    {
        adj.clear();
        deg = 0;
    }
    void incedents(IncidenceCallBack& cb) const
    {
        for (size_t i = 0; i < adj.size(); i++)
            if (adj[i])
                cb(i);
    }
};

class IncidenceVectorInt : public Vertex
{

private:
    std::vector<size_t> adj;
    size_t deg;
public:
    IncidenceVectorInt (): deg(0) {};
    void addNeighbour(size_t v)
    {
        adj[v]++;
        deg++;
    }
    void delNeighbour(size_t v)
    {
        if (v >= adj.size() || adj[v] == 0)
        {
            deletingError();
            return;
        }
        if (adj[v] > 0)
        {
            deg--;
            adj[v]--;
        }
    }
    bool isConnect(size_t v) const
    {
        if (v >= adj.size())
            return false;
        return (adj[v] > 0);
    }
    size_t degree() const
    {
        return deg;
    }
    void clear()
    {
        adj.clear();
        deg = 0;
    }
    void incedents(IncidenceCallBack& cb) const
    {
        for (size_t i = 0; i < adj.size(); i++)
            if (adj[i])
                cb(i);
    }
};

class IncidenceSet : public Vertex
{
private:
    std::set<size_t> adj;
public:
    void addNeighbour(size_t v)
    {
        adj.insert(v);
    }
    void delNeighbour(size_t v)
    {
        std::set<size_t>:: iterator it = adj.find(v);
        if (it == adj.end())
            deletingError();
        else
            adj.erase(it);
    }
    bool isConnect(size_t v) const
    {
        return (adj.find(v) != adj.end());
    }
    size_t degree() const
    {
        return adj.size();
    }
    void clear()
    {
        adj.clear();
    }
    void incedents(IncidenceCallBack& cb) const
    {
        for (auto it = adj.begin(); it != adj.end(); it++)
            cb(*it);
    }
};

class IncidenceMap : public Vertex
{
private:
    std::map<size_t, size_t> adj;
public:
    void addNeighbour(size_t v)
    {
        adj[v]++;
    }
    void delNeighbour(size_t v)
    {
        std::map<size_t, size_t>:: iterator it;
        it = adj.find(v);
        if (it == adj.end())
        {
            deletingError();
            return;
        }
        if (it->second == 1)
            adj.erase(it);
        else
            it->second--;
    }
    bool isConnect(size_t v) const
    {
        return (adj.find(v) != adj.end());
    }
    size_t degree() const
    {
        return adj.size();
    }
    void clear()
    {
        adj.clear();
    }
    void incedents(IncidenceCallBack& cb) const
    {
        for (auto it = adj.begin(); it != adj.end(); it++)
            cb(it->first);
    }
};

class IncidenceList : public Vertex
{
private:
    std::vector<size_t> adj;
public:
    void addNeighbour(size_t v)
    {
        adj.push_back(v);
    }
    void delNeighbour(size_t v)
    {
        bool isDeleted = false;
        for (size_t pos = 0; pos < adj.size(); pos++)
            if (adj[pos] == v)
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
    bool isConnect(size_t v) const
    {

        auto it =
            std::find_if(adj.begin(), adj.end(), [v] (size_t x)
        {
            return (x == v);
        });
        return (it != adj.end());
    }
    size_t degree() const
    {
        return adj.size();
    }
    void clear()
    {
        adj.clear();
    }
    void incedents(IncidenceCallBack& cb) const
    {
        for (auto it = adj.begin(); it != adj.end(); it++)
            cb(*it);
    }
};

#endif
