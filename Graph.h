#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <iostream>
#include <vector>
#include "Incidents.h"

template<typename TWeight>
class Graph
{
private:
    std::vector< std::unique_ptr< Vertex<TWeight> > > graph;
public:
    void addEdge(size_t from, size_t to, const TWeight& weight = Tweight())
    {
        graph[from]->addNeighbour(Edge<TWeight>(to, weight));
    }
    void addUndirEdge(size_t from, size_t to, const TWeight& weight = Tweight())
    {
        addEdge(from, to, weight);
        addEdge(to, from, weight);
    }
    void delEdge(size_t from, size_t to, const TWeight& weight = Tweight())
    {
        graph[from]->delNeighbour(Edge<TWeight>(to, weight));
    }
    void delUndirEdge(size_t from, size_t to, const TWeight& weight = Tweight())
    {
        delEdge(from, to, weight);
        delEdge(to, from, weight);
    }
    void addVertex(std::unique_ptr< Vertex<TWeight> > newElem)
    {
        graph.emplace_back(std::move(newElem));
    }
    size_t numVertices() const
    {
        return graph.size();
    }
    bool isConnect(size_t num1, size_t num2, const TWeight& weight = Tweight()) const
    {
        return graph[num1]->isConnect(Edge<TWeight>(num2, weight));
    }
    void resize(size_t n)
    {
        if (n > graph.size())
        {
            for (size_t i = graph.size(); i < n; ++i)
            {
                std::unique_ptr< Vertex<TWeight> > newVertex(new IncidenceList<TWeight>);
                addVertex(std::move(newVertex));
            }
        }
        else
        {
            for (size_t i = n; i < graph.size(); ++i)
                for (size_t j = 0; j < n; ++j)
                    if (graph[j]->isConnect(i))
                        graph[j]->delNeighbours(i);
            for (; n != graph.size();)
                graph.pop_back();
        }
    }
    void foreachIncidence(size_t num, IncidenceCallBack<TWeight>& cb) const
    {
        graph[num]->incedents(cb);
    }
};

#endif
