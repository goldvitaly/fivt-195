#ifndef GRAPH_ALGORITHM_HPP
#define GRAPH_ALGORITHM_HPP

#include <vector>
#include <memory>
#include <utility>
#include "graph.hpp"

class GraphAlgorithm
{
public:
    std::vector<size_t> FindStronglyConnectedComponents (const Graph& G)
    {
        _Used.assign(G.VertexNum(), false);
        _Comp.assign(G.VertexNum(), 0);
        _Tin.assign(G.VertexNum(), 0);
        _TinMin.assign(G.VertexNum(), 0);
        _InProcess.assign(G.VertexNum(), 0);
        _Stack.clear();
        _Timer = 0;
        _Color = 0;
        
        for (size_t i = 0; i < G.VertexNum(); ++i)
            if (!_Used[i])
                StronglyConnectedComponentsDFS(G, i);
        
        return _Comp;
    }
private:
    size_t _Timer, _Color;
    std::vector<size_t> _Comp, _Tin, _TinMin;
    std::vector<bool> _Used;
    std::vector<size_t> _Stack;
    std::vector<bool> _InProcess;
    
    void StronglyConnectedComponentsDFS (const Graph& G, size_t v)
    {
        _Used[v] = true;
        _Stack.push_back(v);
        _Tin[v] = _TinMin[v] = _Timer++;
        _InProcess[v] = true;
        
        for (auto u : *G.GetIncident(v))
        {
            if (!_Used[u])
            {
                StronglyConnectedComponentsDFS(G, u);
                _TinMin[v] = std::min(_TinMin[v], _TinMin[u]);
            }
            else if (_InProcess[u])
                _TinMin[v] = std::min(_TinMin[v], _Tin[u]);
        }
        
        if (_Tin[v] == _TinMin[v])
        {
            size_t u;
            do
            {
                u = _Stack.back();
                _Stack.pop_back();
                _InProcess[u] = false;
                _Comp[u] = _Color;                
                
            } while (u != v);
            _Color++;
        }
    }
};

#endif /* GRAPH_ALGORITHM_HPP */