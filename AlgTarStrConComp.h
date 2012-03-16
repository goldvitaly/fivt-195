#ifndef ALGTARSTRCONCOMP_H_INCLUDED
#define ALGTARSTRCONCOMP_H_INCLUDED

#include "graph.h"
#include <map>
#include <vector>
#include <iostream>

class AlgTarStrConComp
{
typedef unsigned int TypeNameVer;
public:
    explicit AlgTarStrConComp(const Graph<Vertex>& graph_)
    : graph(graph_), runDFS(*this)
    {
        mark.resize(graph.size(), 0);
        index.resize(graph.size(), 0);
        lowIndex.resize(graph.size(), 0);
        index.resize(graph.size(), 0);
        curInd = 0;
    }
    std::vector<std::vector<TypeNameVer> > str_con_com()
    {
        graph.for_each_vertex(runDFS);
        return components;
    }
private:
    const Graph<Vertex>& graph;
    std::vector<int> mark;
    std::vector<int> index;
    std::vector<int> lowIndex;
    std::vector<TypeNameVer> stack;
    std::vector<std::vector<TypeNameVer> > components;
    int curInd;
    class RunDFS : public UnaryFunc
    {
        typedef unsigned int TypeNameVer;
        AlgTarStrConComp& algTar;
    public:
        RunDFS(AlgTarStrConComp& algTar_) : algTar(algTar_)
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
        AlgTarStrConComp& algTar;
        const TypeNameVer& root;
    public:
        UpdateIndex(AlgTarStrConComp& algTar_, const TypeNameVer& root_)
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
        const int sizeComp = components.size();
        int curSizeStack = stack.size();
        while(stack[curSizeStack - 1] != vertex)
        {
            mark[stack[curSizeStack - 1]] = 2;
            components[sizeComp - 1].push_back(stack[curSizeStack - 1]);
            curSizeStack--;
        }
        mark[stack[curSizeStack - 1]] = 2;
        components[sizeComp - 1].push_back(stack[curSizeStack - 1]);
        stack.resize(curSizeStack - 1);
    }
};


#endif // ALGTARSTRCONCOMP_H_INCLUDED
