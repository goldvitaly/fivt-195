#ifndef STRONGLYCONNECTEDCOMP_H_INCLUDED
#define STRONGLYCONNECTEDCOMP_H_INCLUDED

#include "graph.h"
#include <map>
#include <vector>
#include <iostream>

template<class StructVer, class Weight>
class StronglyConnectedComp
{
typedef unsigned int TypeNameVer;
public:
    explicit StronglyConnectedComp(const Graph<StructVer, Weight>& graph_)
    : graph(graph_), runDFS(*this)
    {
        mark.resize(graph.size());
        index.resize(graph.size());
        lowIndex.resize(graph.size());
        index.resize(graph.size());
        curInd = 0;
    }
    std::vector<std::vector<TypeNameVer> > listComponents()
    {
        graph.for_each_vertex(runDFS);
        return components;
    }
private:
    const Graph<StructVer, Weight>& graph;
    std::vector<int> mark;
    std::vector<int> index;
    std::vector<int> lowIndex;
    std::vector<TypeNameVer> stack;
    std::vector<std::vector<TypeNameVer> > components;
    int curInd;
    class RunDFS : public UnaryFunc
    {
        typedef unsigned int TypeNameVer;
        StronglyConnectedComp& algTar;
    public:
        RunDFS(StronglyConnectedComp& algTar_) : algTar(algTar_)
        {
        }
        void operator()(const TypeNameVer& vertex)
        {
            if(algTar.mark[vertex] == 0)
                algTar.dfs(vertex);
        }
    };
    RunDFS runDFS;
    class UpdateIndex: public UnaryFunc
    {
        typedef unsigned int TypeNameVer;
        StronglyConnectedComp& algTar;
        const TypeNameVer& root;
    public:
        UpdateIndex(StronglyConnectedComp& algTar_, const TypeNameVer& root_)
        : algTar(algTar_) , root(root_)
        {

        }
        void operator()(const TypeNameVer& vertex)
        {
            if(algTar.mark[vertex] == 0)
            {
                algTar.dfs(vertex);
                algTar.lowIndex[root] = std::min(algTar.lowIndex[root], algTar.lowIndex[vertex]);
            }
            else if(algTar.mark[vertex] == 1)
            {
                algTar.lowIndex[root] = std::min(algTar.lowIndex[root], algTar.index[vertex]);
            }
        }
    };
    void dfs(const TypeNameVer& vertex)
    {
        mark[vertex] = 1;
        index[vertex] = curInd;
        lowIndex[vertex] = curInd;
        curInd++;
        stack.push_back(vertex);
        UpdateIndex updateIndex(*this, vertex);
        graph.for_each_neighbour(vertex, updateIndex);
        if(index[vertex] == lowIndex[vertex])
        {
            make_component(vertex);
        }
    }
    void make_component(const TypeNameVer& vertex)
    {
        components.resize(components.size() + 1);
        std::vector<TypeNameVer>&  newComponent = components.back();
        TypeNameVer lastVertex;
        do
        {
            lastVertex = stack.back();
            mark[lastVertex] = 2;
            newComponent.push_back(lastVertex);
            stack.pop_back();
        } while(lastVertex != vertex);
   }
};


#endif // STRONGLYCONNECTEDCOMP_H_INCLUDED
