#ifndef GRAPH_ALGORITHM_HPP
#define GRAPH_ALGORITHM_HPP

#include <vector>
#include <memory>
#include <utility>
#include "graph.hpp"

class GraphAlgorithm
{
public:
    struct VertexInfo
    {
        size_t Tin;
        size_t TinMin;
        bool Used;
        bool InProcess;
        VertexInfo()
        {
            Tin = 0;
            TinMin = 0;
            Used = 0;
            InProcess = 0;
        }
    };
    std::vector<size_t> FindStronglyConnectedComponents (const Graph& G)
    {
        _Vertex.assign(G.VertexNum(), VertexInfo());
        _Comp.assign(G.VertexNum(), 0);
        _Stack.clear();
        _Timer = 0;
        _Color = 0;
        
        for (size_t i = 0; i < G.VertexNum(); ++i)
            if (!_Vertex[i].Used)
                StronglyConnectedComponentsDFS(G, i);
        
        return _Comp;
    }
private:
    size_t _Timer, _Color;
    std::vector<VertexInfo> _Vertex;
    std::vector<size_t> _Comp;
    std::vector<size_t> _Stack;
    
    void StronglyConnectedComponentsDFS (const Graph& G, size_t v)
    {
        _Vertex[v].Used = true;
        _Stack.push_back(v);
        _Vertex[v].Tin = _Vertex[v].TinMin = _Timer++;
        _Vertex[v].InProcess = true;
        
        for (auto u : G.GetIncident(v))
        {
            if (!_Vertex[u].Used)
            {
                StronglyConnectedComponentsDFS(G, u);
                _Vertex[v].TinMin = std::min(_Vertex[v].TinMin, _Vertex[u].TinMin);
            }
            else if (_Vertex[u].InProcess)
                _Vertex[v].TinMin = std::min(_Vertex[v].TinMin, _Vertex[u].Tin);
        }
        
        if (_Vertex[v].Tin == _Vertex[v].TinMin)
        {
            size_t u;
            do
            {
                u = _Stack.back();
                _Stack.pop_back();
                _Vertex[u].InProcess = false;
                _Comp[u] = _Color;                
                
            } while (u != v);
            _Color++;
        }
    }
};

#endif /* GRAPH_ALGORITHM_HPP */
