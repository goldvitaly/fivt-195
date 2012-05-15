#ifndef MAXFLOW_H_INCLUDED
#define MAXFLOW_H_INCLUDED

#include <map>
#include <utility>
#include <algorithm>


#include "graph.h"
#include "VertexVector.h"
#include "ShortestPath.h"


template<class StructVer, class Weight, class Cmp>
class MaxFlow
{
typedef unsigned int TypeNameVer;
public:
    explicit MaxFlow(const Graph<StructVer, Weight>& graph_)
    : graph(graph_), runDFS(*this)
    {
        mark.resize(graph.size());
        make_copy_graph();
    }
    Weight calculate(const TypeNameVer& source_, const TypeNameVer& target_)
    {
        source = source_;
        target = target_;
        return searchMaxFlow();
    }
private:
    const Graph<StructVer, Weight>& graph;
    Graph<Vertex<int>, int> residualGraph;
    TypeNameVer source, target;
    std::vector<int> mark;
    std::vector<Weight> capacityEdge;

    class RunDFS : public UnaryFunc
    {
        typedef unsigned int TypeNameVer;
        MaxFlow& maxFlow;
    public:
        RunDFS(MaxFlow& maxFlow_) : maxFlow(maxFlow_)
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
        void operator()(const TypeNameVer& vertex, const Weight& weight)
        {
            maxFlow.capacityEdge.push_back(weight);
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
        Weight notExistPath;
    public:
        CalculatePath(const MaxFlow& maxFlow_, Weight notExistPath_) : maxFlow(maxFlow_)
        {
            notExistPath = notExistPath_;
        }
        Weight operator()(AccessPath<Weight, Weight> accessPath)
        {
            if(accessPath.prev() == NULL)
            {
                return  accessPath.path();
            }
            else if(accessPath.prev()->prev() == NULL)
                return maxFlow.capacityEdge[accessPath.weight()];
            else
                return std::min(accessPath.path(), maxFlow.capacityEdge[accessPath.weight()]);
        }
    };

    class InvCmp
    {
    public:
        bool operator()( Weight elem1, Weight elem2)
        {
            return Cmp()(elem2, elem1);
        }
    };

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

    Weight searchMaxFlow()
    {
        Weight maxFlow = 0;
        Weight delta;
        int id = 0;
        do
        {
            id++;
            typedef ShortestPath<Vertex<int>, int, Weight, CalculatePath, InvCmp> MyShortestPath;
            MyShortestPath shortestPath(residualGraph, CalculatePath(*this, -1));
            typename MyShortestPath::VectorAccessPath vectorAccessPath = shortestPath.calculate(source, -1);
            AccessPath<int, int> accessPath = vectorAccessPath[target];
            delta = CalculatePath(*this, -1)(accessPath);
            while(accessPath.prev() != NULL)
            {
                capacityEdge[accessPath.weight()] -= delta;
                capacityEdge[(accessPath.weight() % 2 == 0) ? (accessPath.weight() + 1) : (accessPath.weight() - 1)] += delta;
                accessPath = *accessPath.prev();
            }
            maxFlow += delta;

        }while(Cmp()(0, delta) && id <10);
        return maxFlow;
    }
};


#endif // MAXFLOW_H_INCLUDED
