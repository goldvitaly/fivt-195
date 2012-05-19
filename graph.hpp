#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <memory>
#include <utility>
#include "incident.hpp"

template<typename EdgeWeight> 
class Graph
{
public:
    Graph () {}
    template <typename IncidentClass> size_t AddVertex ()
    {
        _Inc.push_back(std::unique_ptr<IncidentClass>(new IncidentClass()));
        return _Inc.size() - 1;
    }
    void RemoveVertex (size_t Vertex)
    {
        if (Vertex >= _Inc.size()) return;

        for (size_t v = 0; v < _Inc.size(); ++v)
        {
            (*_Inc[v]).RemoveIncident(Vertex);
        }

        (*_Inc[Vertex]).clear();
    }
    void AddIncident (size_t VertexA, size_t VertexB, EdgeWeight edge)
    {
        _Inc[VertexA]->AddIncident(std::make_pair(VertexB, edge));
    }
    bool CheckIncident (size_t VertexA, size_t VertexB) const
    {
        if (VertexA >= _Inc.size()) return false;
        return _Inc[VertexA]->CheckIncident(VertexB);
    }
    size_t VertexNum () const
    {
        return _Inc.size();
    }
    size_t IncidentNum () const
    {
        size_t num = 0;
        for (size_t i = 0; i < _Inc.size(); ++i)
            num += _Inc[i]->IncidentNum();
        return num;
    }
    const Incident<EdgeWeight>& GetIncident (size_t Vertex) const
    {
        return *_Inc[Vertex];
    }
private:
    std::vector <std::unique_ptr<Incident<EdgeWeight>>> _Inc;
};

#endif /* GRAPH_HPP */
