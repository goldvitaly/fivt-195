#ifndef SHORTESTPATH_H_INCLUDED
#define SHORTESTPATH_H_INCLUDED

#include "graph.h"
#include <map>
#include <vector>
#include <iostream>

// добавить структура пути, функция пересчета( доступ к пути, стр пути у нашей вершины, вес ребра в след), компаратор, вернуть его структуру
// сет или куча а то медленно

template<class StructVer, class Weight>
class ShortestPath
{
typedef unsigned int TypeNameVer;
public:
    int count(int vertexBegin, int vertexEnd)
    {
        begin(vertexBegin);
        return (Mark[vertexEnd] == 2) ? Dist[vertexEnd] : -1;
    }
    std::vector<int> count(int vertex)
    {
        begin(vertex);
        return Dist;
    }
    ShortestPath(const Graph<StructVer, Weight>& graph_) : graph(graph_)
    {
        Dist.resize(graph.size());
        Mark.resize(graph.size());
    }
private:
    const Graph<StructVer, Weight>& graph;
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
            Relax relax(*this, graph, id_min);
            graph.for_each_neighbour((TypeNameVer)id_min, relax);
        }
    }
    class Relax : public UnaryFunc
    {
        typedef unsigned int TypeNameVer;
        ShortestPath& shortestPath;
        const Graph<StructVer, Weight>& graph;
        TypeNameVer root;
        public:
        Relax(ShortestPath& shortestPath_, const Graph<StructVer, Weight>& graph_, TypeNameVer root_) : shortestPath(shortestPath_) , graph(graph_)
        {
            root = root_;
        }
        void operator()(const TypeNameVer& vertex)
        {
            if(shortestPath.Mark[vertex] != 2 &&
              (shortestPath.Mark[vertex] == 0 || shortestPath.Dist[root] + graph.weight(root, vertex) < shortestPath.Dist[vertex]))
            {
                shortestPath.Mark[vertex] = 1;
                shortestPath.Dist[vertex] = shortestPath.Dist[root] + graph.weight(root, vertex);
            }
        }
    };
};



#endif // SHORTESTPATH_H_INCLUDED
