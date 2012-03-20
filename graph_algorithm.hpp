#ifndef GRAPH_ALGORITHM_HPP
#define GRAPH_ALGORITHM_HPP

#include <vector>
#include <memory>
#include <utility>
#include "graph.hpp"

class GraphAlgorithm
{
public:
    std::vector<size_t> FindStronglyConnectedComponents (const Graph& G);
private:
    size_t _Timer, _Color;
    std::vector<size_t> _Comp, _Tin;
    std::vector<bool> _Used;
    std::vector<size_t> _Stack;
    
    void StronglyConnectedComponentsDFS (const Graph& G, size_t v);
};

#endif /* GRAPH_ALGORITHM_HPP */