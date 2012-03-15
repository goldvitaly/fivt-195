#ifndef ALGTARSTRCONCOMP_H_INCLUDED
#define ALGTARSTRCONCOMP_H_INCLUDED

#include "graph.h"
#include <map>
#include <vector>
#include <iostream>

template<class TypeNameVer>
class AlgTarStrConComp
{
public:
    AlgTarStrConComp(Graph<TypeNameVer, Vertex<TypeNameVer> >& graph_)
    : graph(graph_), runDFS(this)
    {
        curInd = 0;
    }
    std::vector<std::vector<TypeNameVer> > str_con_com()
    {
        graph.for_each_vertex(runDFS);
        return components;
    }
private:
    Graph<TypeNameVer, Vertex<TypeNameVer> >& graph;
    std::map<TypeNameVer, int> mark;
    std::map<TypeNameVer, int> index;
    std::map<TypeNameVer, int> lowIndex;
    std::vector<TypeNameVer> stack;
    std::vector<std::vector<TypeNameVer> > components;
    int curInd;
    class RunDFS : public UnaryFunc<TypeNameVer>
    {
        AlgTarStrConComp<TypeNameVer>* Alg2;
    public:
        RunDFS(AlgTarStrConComp<TypeNameVer>* Alg2_) : Alg2(Alg2_)
        {}
        void operator()(TypeNameVer vertex)
        {
            if(Alg2->mark[vertex] == 0)
                Alg2->dfs(vertex);
        }
    } runDFS;
    class UpdateIndex: public UnaryFunc<TypeNameVer>
    {
        AlgTarStrConComp<TypeNameVer>* algTar;
        const TypeNameVer& root;
    public:
        UpdateIndex(AlgTarStrConComp<TypeNameVer>* algTar_, const TypeNameVer& root_)
        : root(root_)
        {
            algTar = algTar_;
        }
        void operator()(TypeNameVer vertex)
        {
            if(algTar->mark[vertex] == 0)
            {
                algTar->dfs(vertex);
                algTar->lowIndex[root] = std::min(algTar->lowIndex[root], algTar->lowIndex[vertex]);
            }
            else if(algTar->mark[vertex] == 1)
            {
                algTar->lowIndex[root] = std::min(algTar->lowIndex[root], algTar->index[vertex]);
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
        UpdateIndex updateIndex(this, vertex);
        graph.for_each_neighbour(vertex, updateIndex);
        if(index[vertex] == lowIndex[vertex])
        {
            make_component(vertex);
        }
        mark[vertex] = 2;
    }
    void make_component(const TypeNameVer& vertex)
    {
        components.resize(components.size() + 1);
        const int sizeComp = components.size();
        int curSizeStack = stack.size();
        while(stack[curSizeStack - 1] != vertex)
        {
            components[sizeComp - 1].push_back(stack[curSizeStack - 1]);
            curSizeStack--;
        }
        components[sizeComp - 1].push_back(stack[curSizeStack - 1]);
        stack.resize(curSizeStack - 1);
    }
};


#endif // ALGTARSTRCONCOMP_H_INCLUDED
