#ifndef MAXFLOW_H_INCLUDED
#define MAXFLOW_H_INCLUDED

#include <utility>
#include <algorithm>
#include <vector>

#include "graph.h"
#include "VertexVector.h"
#include "ShortestPath.h"


template<class StructVer, class Capacity, class Cmp>
class MaxFlow
{
typedef unsigned int TypeNameVer;
public:
    explicit MaxFlow(const Graph<StructVer, Capacity>& graph_)
    : graph(graph_), runDFS(*this), notExistsPath(0), calculatePath(*this, notExistsPath)
    {
        mark.resize(graph.size());
        make_copy_graph();
    }
    Capacity calculate(const TypeNameVer& source_, const TypeNameVer& target_)
    {
        source = source_;
        target = target_;
        return searchMaxFlow();
    }
private:
    const Graph<StructVer, Capacity>& graph;
    Graph<Vertex<int>, int> residualGraph;
    TypeNameVer source, target;
    std::vector<int> mark;
    std::vector<Capacity> capacityEdge;
    Capacity notExistsPath;

    class RunDFS : public UnaryFunc
    {
        typedef unsigned int TypeNameVer;
        MaxFlow& maxFlow;
    public:
        explicit RunDFS(MaxFlow& maxFlow_) : maxFlow(maxFlow_)
        {
        }
        void operator()(const TypeNameVer& vertex)
        {
            if(maxFlow.mark[vertex] == 0)
                maxFlow.dfs(vertex);
        }
    };
    RunDFS runDFS;

    class AddEdge: public BinaryFunc<int>
    {
        typedef unsigned int TypeNameVer;
        MaxFlow& maxFlow;
        const TypeNameVer& root;
    public:
        AddEdge(MaxFlow& maxFlow_, const TypeNameVer& root_)
        : maxFlow(maxFlow_) , root(root_)
        {
        }
        void operator()(const TypeNameVer& vertex, const Capacity& capacity)
        {
            maxFlow.capacityEdge.push_back(capacity);
            maxFlow.residualGraph.add_edge(root, vertex, maxFlow.capacityEdge.size()-1);
            maxFlow.capacityEdge.push_back(0);
            maxFlow.residualGraph.add_edge(vertex, root, maxFlow.capacityEdge.size()-1);
            if(maxFlow.mark[vertex] == 0)
                maxFlow.dfs(vertex);
        }
    };

    class CalculatePath
    {
        const MaxFlow& maxFlow;
        Capacity notExistPath;
    public:
        CalculatePath(const MaxFlow& maxFlow_, const Capacity& notExistPath_) : maxFlow(maxFlow_)
        {
            notExistPath = notExistPath_;
        }
        Capacity operator()(const AccessPath<Capacity, Capacity>& accessPath)
        {
            if(accessPath.prev() == NULL)
                return accessPath.path();
            else if(accessPath.prev()->prev() == NULL)
                return maxFlow.capacityEdge[accessPath.weight()];
            else
                return std::min(accessPath.path(), maxFlow.capacityEdge[accessPath.weight()]);
        }
    };
    CalculatePath calculatePath;

    void make_copy_graph()
    {
        for(int i = 0; i < graph.size(); i++)
            residualGraph.add_vertex(i, new VertexVector<int>());
        graph.for_each_vertex(runDFS);
    }

    void dfs(const TypeNameVer& vertex)
    {
        mark[vertex] = 1;
        AddEdge addEdge(*this, vertex);
        graph.for_each_neighbour(vertex, addEdge);
    }

    class InvCmp
    {
    public:
        bool operator()(const Capacity& elem1, const Capacity& elem2) const
        {
            return Cmp()(elem2, elem1);
        }
    };

    Capacity searchMaxFlow()
    {
        Capacity maxFlow = 0;
        Capacity delta;
        do
        {
            typedef ShortestPath<Vertex<int>, int, Capacity, CalculatePath, InvCmp> MyShortestPath;

            MyShortestPath shortestPath(residualGraph, calculatePath);
            typename MyShortestPath::VectorAccessPath vectorAccessPath = shortestPath.calculate(source, notExistsPath);
            AccessPath<int, int> accessPath = vectorAccessPath[target];

            delta = calculatePath(accessPath);
            while(accessPath.prev() != NULL)
            {
                capacityEdge[accessPath.weight()] -= delta;
                capacityEdge[inv_edge(accessPath.weight())] += delta;
                accessPath = *accessPath.prev();
            }
            maxFlow += delta;

        }while(Cmp()(0, delta));
        return maxFlow;
    }

    int inv_edge(int numEdge) const
    {
        return (numEdge % 2 == 0) ? (numEdge + 1) : (numEdge - 1);
    }
};


#endif // MAXFLOW_H_INCLUDED
