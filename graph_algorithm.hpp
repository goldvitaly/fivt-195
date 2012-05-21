#ifndef GRAPH_ALGORITHM_HPP
#define GRAPH_ALGORITHM_HPP

#include <iostream>
#include <cassert>
#include <vector>
#include <memory>
#include <utility>
#include "graph.hpp"

class Edge
{
public:
    Edge(): weight(0){}
    explicit Edge(size_t Weight): weight(Weight){}
    size_t operator () () const
    {
        return weight;
    }
private:
    size_t weight;
};

template<typename EdgeWeight>
class Path
{    
public:
    Path(): weight(0), edge(){};
    Path(const Path &p): weight(p()), edge(p.GetPath()){}
    size_t operator () () const
    {
        return weight;
    }
    void AddEdge (const EdgeWeight &e, size_t EdgeIndex)
    {
        weight += e();
        edge.push_back(EdgeIndex);
    }
    const std::vector<size_t>& GetPath () const
    { 
        return edge;
    }
private:
    size_t weight;
    std::vector<size_t> edge;
};

template<typename PathWeight>
class PathComp
{
public:
    PathComp(){};
    bool operator () (const PathWeight &a, const PathWeight &b) const
    {
        return a() < b();
    }
private:
};

template<typename EdgeWeight, typename PathWeight>
class Relax
{
public:
    Relax(){};
    PathWeight operator () (const PathWeight &p, const EdgeWeight &e, size_t EdgeIndex) const
    {
        PathWeight ans(p);
        ans.AddEdge(e, EdgeIndex);
        return ans;
    }
private:
};

template<typename EdgeWeight, typename PathWeight, typename PathWeightComp, typename RelaxWeight>
class DijkstraAlgorithm
{
public:
    DijkstraAlgorithm (): undefinedVertex(0), inProcessVertex(1), processedVertex(2){}
    PathWeight operator () (const Graph<EdgeWeight> &G, size_t start, size_t finish)
    {
        state.resize(G.VertexNum(), undefinedVertex);
        pathWeight.resize(G.VertexNum());
        
        state[start] = inProcessVertex;
        
        for (size_t Iter = 0; Iter < G.VertexNum(); ++Iter)
        {
            bool found = false;
            size_t fndInd = 0;
            
            for (size_t v = 0; v < G.VertexNum(); ++v)
            {
                if (state[v] == inProcessVertex)
                {
                    if (!found || cmp(pathWeight[v], pathWeight[fndInd]))
                    {
                        fndInd = v;
                        found = true;
                    }
                }
            }
            
            size_t EdgeIndex = 0;
            for (auto v : G.GetIncident(fndInd))
            {
                if (state[v.first] != processedVertex && 
                    (state[v.first] == undefinedVertex ||
                     cmp(relax(pathWeight[fndInd], v.second, EdgeIndex), pathWeight[v.first])))
                {
                    state[v.first] = inProcessVertex;
                    pathWeight[v.first] = PathWeight(relax(pathWeight[fndInd], v.second, EdgeIndex));
                }
                EdgeIndex++;
            }
            
            state[fndInd] = processedVertex;
        }
    
        return pathWeight[finish];
    }
private:
    std::vector<size_t> state;
    std::vector<PathWeight> pathWeight;
    PathWeightComp cmp;
    RelaxWeight relax;
    const size_t undefinedVertex;
    const size_t inProcessVertex;
    const size_t processedVertex;
};

#endif /* GRAPH_ALGORITHM_HPP */
