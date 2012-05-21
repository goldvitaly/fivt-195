#ifndef ALGDEIKSTRA_H_INCLUDED
#define ALGDEIKSTRA_H_INCLUDED

#include "graph.h"
#include <map>
#include <vector>
#include <iostream>


class AlgDijkstra
{
typedef unsigned int TypeNameVer;
public:
    int count(int vertexBegin, int vertexEnd)
    {
        begin(vertexBegin);
        return (Mark[vertexEnd] == 2) ? Dist[vertexEnd] : -1;
    }
    std::vector<int>& count(int vertex)
    {
        begin(vertex);
        return Dist;
    }
    AlgDijkstra(const Graph<Vertex>& graph_) : graph(graph_)
    {
        Dist.resize(graph.size());
        Mark.resize(graph.size());
    }
private:
    const Graph<Vertex>& graph;
    std::vector<int> Dist;
    std::vector<int> Mark;
    void begin(int vertex)
    {
        Mark[vertex] = 1;
        Dist[vertex] = 0;
        while(1)
        {
            int id_min = -1;
            for(int i = 0; i < Dist.size(); i++)
            {
                if(Mark[i] == 1 && (id_min == -1 || Dist[i] < Dist[id_min]))
                {
                    id_min = i;
                }
            }
            if(id_min == -1)
                break;
            Mark[id_min] = 2;
            std::cout << "take black " << id_min << std::endl;
            Relax relax(*this, id_min);
            graph.for_each_neighbour((TypeNameVer)id_min, relax);
        }
    }
    class Relax : public UnaryFunc
    {
        typedef unsigned int TypeNameVer;
        AlgDijkstra& algDijkstra;
        int root;
        public:
        Relax(AlgDijkstra& algDijkstra_, int root_) : algDijkstra(algDijkstra_)
        {
            root = root_;
        }
        void operator()(const TypeNameVer& vertex)
        {
            if(algDijkstra.Mark[vertex] != 2)
            {
                algDijkstra.Mark[vertex] = 1;
                algDijkstra.Dist[vertex] = algDijkstra.Dist[root] + 1;
            }
        }
    };
};

#endif // ALGDEIKSTRA_H_INCLUDED
