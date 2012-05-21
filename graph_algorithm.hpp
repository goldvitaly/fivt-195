#ifndef GRAPH_ALGORITHM_HPP
#define GRAPH_ALGORITHM_HPP

#include <iostream>
#include <set>
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
        
        std::set<std::pair<PathWeight, size_t>, DijkstraComp> vertexQueue;
        vertexQueue.insert(std::make_pair(pathWeight[start], start));
        
        while (!vertexQueue.empty())
        {
            size_t fndInd = vertexQueue.begin()->second;
            vertexQueue.erase(vertexQueue.begin());
            
            size_t EdgeIndex = 0;
            for (auto v : G.GetIncident(fndInd))
            {
                if (state[v.first] != processedVertex && 
                    (state[v.first] == undefinedVertex ||
                     cmp(relax(pathWeight[fndInd], v.second, EdgeIndex), pathWeight[v.first])))
                {
                    vertexQueue.erase(std::make_pair(pathWeight[v.first], v.first));
                    vertexQueue.insert(std::make_pair(relax(pathWeight[fndInd], v.second, EdgeIndex), v.first));
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
    class DijkstraComp
    {
    public:
        DijkstraComp(): cmp(){}
        bool operator () (const std::pair<PathWeight, size_t> &a, const std::pair<PathWeight, size_t> &b)
        {
            if (cmp(a.first, b.first)) return true;
            if (cmp(b.first, a.first)) return false;
            return a.second < b.second;
        }
    private:
        PathWeightComp cmp;
    };
    std::vector<size_t> state;
    std::vector<PathWeight> pathWeight;
    PathWeightComp cmp;
    RelaxWeight relax;
    const size_t undefinedVertex;
    const size_t inProcessVertex;
    const size_t processedVertex;
};

#endif /* GRAPH_ALGORITHM_HPP */
