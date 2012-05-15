#ifndef MAXFLOW_H_INCLUDED
#define MAXFLOW_H_INCLUDED

#include <map>
#include <utility>
#include <algorithm>


#include "graph.h"
#include "VertexVector.h"
#include "ShortestPath.h"

//создать граф, где ребра - это номера в массиве. Ивсе данные вынесены.
// O(1).
//можно считать, что для чисел - пропускной способности.

template<class StructVer, class Weight>
class MaxFlow
{
typedef unsigned int TypeNameVer;
public:
    explicit MaxFlow(const Graph<StructVer, Weight>& graph_)
    : graph(graph_), runDFS(*this)
    {
        mark.resize(graph.size());
        make_copy_graph();
        //std::cout << "Ok!" <<" "<< capacityEdge.size() << std::endl;
        //for(int i = 0; i < capacityEdge.size(); i++)
       // {
        //    std::cout << capacityEdge[i] << " ";
       // }
       // std::cout << std::endl;
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
            maxFlow.residualGraph.add_edge(root, vertex, maxFlow.capacityEdge.size());
            maxFlow.capacityEdge.push_back(0);
            maxFlow.residualGraph.add_edge(root, vertex, maxFlow.capacityEdge.size());
            if(maxFlow.mark[vertex] == 0)
                maxFlow.dfs(vertex);
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
};


#endif // MAXFLOW_H_INCLUDED
