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
    explicit Edge(int Weight): weight(Weight){}
    int operator () () const
    {
        return weight;
    }
private:
    int weight;
};

template<typename EdgeWeight>
class Path
{    
public:
    Path(): weight(1e9), edge(){};
    Path(const Path &p): weight(p()), edge(p.GetPath()){}
    int operator () () const
    {
        return weight;
    }
    void AddEdge (const EdgeWeight &e, size_t EdgeIndex)
    {
        weight = std::min(weight, e());
        edge.push_back(EdgeIndex);
    }
    const std::vector<size_t>& GetPath () const
    { 
        return edge;
    }
private:
    int weight;
    std::vector<size_t> edge;
};

template<typename PathWeight>
class PathComp
{
public:
    PathComp(){};
    bool operator () (const PathWeight &a, const PathWeight &b) const
    {
        return a() > b();
    }
private:
};

template<typename EdgeWeight, typename PathWeight>
class Relax
{
public:
    std::vector<std::vector<std::unique_ptr<EdgeWeight>>> flowWeight;
    Relax(const Graph<EdgeWeight> &G)
    {
        flowWeight.resize(G.VertexNum());
        for (size_t i = 0; i < G.VertexNum(); ++i)
        {
            flowWeight[i].resize(G.GetIncident(i).IncidentNum());
        }
    }
    PathWeight operator () (const PathWeight &p, const EdgeWeight &e, size_t Vertex, size_t EdgeIndex) const
    {
        PathWeight ans(p);
        if (flowWeight[Vertex][EdgeIndex] == nullptr)
        {
            ans.AddEdge(e, EdgeIndex);
        }
        else
        {
            ans.AddEdge(EdgeWeight(e() - (*flowWeight[Vertex][EdgeIndex])()), EdgeIndex);            
        }
        
        return ans;
    }
private:
};

template<typename EdgeWeight, typename PathWeight, 
    typename PathWeightComp, typename RelaxWeight>
class DijkstraAlgorithm
{
public:
    DijkstraAlgorithm (): undefinedVertex(0), inProcessVertex(1), processedVertex(2){}
    PathWeight operator () (const Graph<EdgeWeight> &G, size_t Start, size_t Finish, const RelaxWeight &relax)
    {
        state.resize(G.VertexNum(), undefinedVertex);
        pathWeight.resize(G.VertexNum());
    
        state[Start] = inProcessVertex;
        
        std::set<std::pair<PathWeight, size_t>, DijkstraComp> vertexQueue;
        vertexQueue.insert(std::make_pair(pathWeight[Start], Start));
        
        while (!vertexQueue.empty())
        {
            size_t fndInd = vertexQueue.begin()->second;
            vertexQueue.erase(vertexQueue.begin());
            
            size_t EdgeIndex = 0;
            for (auto v : G.GetIncident(fndInd))
            {
                if (state[v.first] != processedVertex && 
                    (state[v.first] == undefinedVertex ||
                     cmp(relax(pathWeight[fndInd], v.second, fndInd, EdgeIndex), pathWeight[v.first])))
                {
                    vertexQueue.erase(std::make_pair(pathWeight[v.first], v.first));
                    vertexQueue.insert(std::make_pair(relax(pathWeight[fndInd], v.second, fndInd, EdgeIndex), v.first));
                    state[v.first] = inProcessVertex;
                    pathWeight[v.first] = PathWeight(relax(pathWeight[fndInd], v.second, fndInd, EdgeIndex));
                }
                EdgeIndex++;
            }
            
            state[fndInd] = processedVertex;
        }
        
        state.resize(0);
        pathWeight.resize(0);
        return pathWeight[Finish];
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
    const size_t undefinedVertex;
    const size_t inProcessVertex;
    const size_t processedVertex;
};

template<typename EdgeWeight, typename PathWeight, 
    typename PathWeightComp, typename RelaxWeight, typename FlowType>
class MaxFlowAlgorithm
{
public:
    MaxFlowAlgorithm (const Graph<EdgeWeight> &G): g(G), relax(G){}
    FlowType operator () (size_t Start, size_t Finish)
    {
        flow = FlowType();
        
        DijkstraAlgorithm<EdgeWeight, PathWeight, PathWeightComp, RelaxWeight> da;            
        PathWeight pathWeight;
        FlowType cFlow;
        
        while ((pathWeight = da(g, Start, Finish, relax))() > 0)
        {
            pathWeight = da(g, Start, Finish, relax);
            cFlow = pathWeight();
            
            size_t vertex = Start;
            for (auto v : pathWeight.GetPath())
            {
                addFlow(vertex, v, cFlow);
                size_t edgeIndex = 0, nextVertex = g.GetIncident(vertex).GetIncident(v).first;
                for (auto u : g.GetIncident(nextVertex))
                {
                    if (u.first == vertex)
                    {
                        addFlow(nextVertex, edgeIndex, -cFlow);
                        break;
                    }
                    edgeIndex++;
                }
                vertex = nextVertex;
            }
            flow += cFlow;
        }
        
        return flow;
    }
private:
    void addFlow (size_t Vertex, size_t EdgeIndex, FlowType Flow)
    {
        if (relax.flowWeight[Vertex][EdgeIndex] == nullptr)
        {
            relax.flowWeight[Vertex][EdgeIndex] = 
                std::unique_ptr<EdgeWeight>(new EdgeWeight(Flow));
        }
        else
        {
            (*relax.flowWeight[Vertex][EdgeIndex]) = 
                EdgeWeight((*relax.flowWeight[Vertex][EdgeIndex])() + Flow);
        }
    }
    const Graph<EdgeWeight> &g;
    FlowType flow;
    RelaxWeight relax;
};

#endif /* GRAPH_ALGORITHM_HPP */
