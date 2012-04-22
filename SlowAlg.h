#ifndef SLOWALG_H_INCLUDED
#define SLOWALG_H_INCLUDED

#include "graph.h"
#include <vector>
#include <iostream>

template<class StructVer, class Weight>
class SlowAlg{
    typedef unsigned int TypeNameVer;
public:
    explicit SlowAlg(const Graph<StructVer, Weight>& graph_)
    : graph(graph_), mark(graph.size(), std::vector<TypeNameVer>(graph.size())),
     runFirstDFS(*this)
    {

    }
    std::vector<std::vector<TypeNameVer> > listComponents()
    {
        graph.for_each_vertex(runFirstDFS);
        count_components();
        return components;
    }
private:
    const Graph<StructVer, Weight>& graph;
    std::vector<std::vector<TypeNameVer> > mark;
    std::vector<std::vector<TypeNameVer> > components;
    class RunDFS : public UnaryFunc
    {
        typedef unsigned int TypeNameVer;
        SlowAlg& slowAlg;
        TypeNameVer startVertex;
        bool firstDFS;
    public:
        explicit RunDFS(SlowAlg& slowAlg_, bool firstDFS_ = true, TypeNameVer startVertex_ = 0)
         : slowAlg(slowAlg_)
        {
            startVertex = startVertex_;
            firstDFS = firstDFS_;
        }
        void operator()(const TypeNameVer& vertex)
        {
            if(firstDFS == true)
            {
                if(slowAlg.mark[vertex][vertex] == 0)
                    slowAlg.dfs(vertex, vertex);
            }
            else
            {
                if(slowAlg.mark[startVertex][vertex] == 0)
                    slowAlg.dfs(startVertex, vertex);
            }
        }
    };
    RunDFS runFirstDFS;
    void dfs(const TypeNameVer& startVertex, const TypeNameVer& curVertex)
    {
        RunDFS runDFS(*this, false, startVertex);
        mark[startVertex][curVertex] = 1;
        graph.for_each_neighbour(curVertex, runDFS);
    }
    void count_components()
    {
        for(size_t i = 0; i < graph.size(); i++)
        {
            if(mark[i][i] == 1)
            {
                components.resize(components.size() + 1);
                for(size_t j = 0; j < graph.size(); j++)
                {
                    if(mark[i][j] == 1 && mark[j][i] == 1 && i != j)
                    {
                        components[components.size() - 1].push_back(j);
                        mark[j][j] = 0;
                    }
                }
                components[components.size() - 1].push_back(i);
                mark[i][i] = 0;
            }
        }
    }
};

#endif // SLOWALG_H_INCLUDED
