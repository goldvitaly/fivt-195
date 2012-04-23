#ifndef SHORTESTPATH_H_INCLUDED
#define SHORTESTPATH_H_INCLUDED

#include "graph.h"
#include <map>
#include <vector>
#include <iostream>

// добавить структура пути, функция пересчета(доступ к пути, стр пути у нашей вершины, вес ребра в след)
template<class StructVer, class Weight, class Cmp = std::less<Weight> >
class ShortestPath
{
typedef unsigned int TypeNameVer;
typedef typename std::set<std::pair<Weight, TypeNameVer> >::iterator SetIterator;
typedef std::pair<Weight, TypeNameVer> Path;
public:
    Weight count(const TypeNameVer& vertexStart, const TypeNameVer& vertexEnd, const Weight& weightNotPath)
    {
        begin(vertexStart);
        return (Mark[vertexEnd] == 2) ? Dist[vertexEnd] : weightNotPath;
    }
    std::vector<Weight> count(const TypeNameVer& vertex)
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
    class CmpPath{
    public:
        bool operator()(const Path& a, const Path& b) const
        {
            if(Cmp()(a.first, b.first))
                return true;
            else if(Cmp()(b.first, a.first) && a.second < b.second)
                return true;
            else
                return false;
        }
    };
    const Graph<StructVer, Weight>& graph;
    std::vector<Weight> Dist;
    std::vector<int> Mark;
    std::set<Path, CmpPath> set;
    void begin(const TypeNameVer& vertex)
    {
        set.insert(std::make_pair(0, vertex));
        Mark[vertex] = 1;
        while(1)
        {
            if(set.empty())
                break;
            Path minElem = *set.begin();
            Mark[minElem.second] = 2;
            set.erase(set.begin());
            Relax relax(*this, minElem.second);
            graph.for_each_neighbour(minElem.second, relax);
        }
    }
    class Relax : public BinaryFunc<Weight>
    {
        typedef unsigned int TypeNameVer;
        ShortestPath& shortestPath;
        const TypeNameVer& root;
        public:
        Relax(ShortestPath& shortestPath_, const TypeNameVer& root_) : shortestPath(shortestPath_), root(root_)
        {

        }
        void operator()(const TypeNameVer& vertex, const Weight& weight)
        {
            if(shortestPath.Mark[vertex] == 0)
            {
                shortestPath.Mark[vertex] = 1;
                shortestPath.Dist[vertex] = shortestPath.Dist[root] + weight;
                shortestPath.set.insert(std::make_pair(shortestPath.Dist[vertex], vertex));
            }
            else if(shortestPath.Mark[vertex] == 1 &&
              shortestPath.Dist[root] + weight < shortestPath.Dist[vertex])
            {
                shortestPath.set.erase(std::make_pair(shortestPath.Dist[vertex], vertex));
                shortestPath.Dist[vertex] = shortestPath.Dist[root] + weight;
                shortestPath.set.insert(std::make_pair(shortestPath.Dist[vertex], vertex));
            }
        }
    };
};

#endif // SHORTESTPATH_H_INCLUDED
