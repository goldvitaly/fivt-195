#ifndef SHORTESTPATH_H_INCLUDED
#define SHORTESTPATH_H_INCLUDED

#include "graph.h"
#include <map>
#include <vector>
#include <iostream>

// функция пересчета(доступ к пути)
template<class StructVer, class Weight, class Path, class CalkPath, class Cmp = std::less<Path>, class StayingPath = Path>
class ShortestPath
{
typedef unsigned int TypeNameVer;
typedef typename std::set<std::pair<Path, TypeNameVer> >::iterator SetIterator;
typedef std::pair<Path, TypeNameVer> CurPath;
public:
    Path count(const TypeNameVer& vertexStart, const TypeNameVer& vertexEnd, const Path& notPath)
    {
        begin(vertexStart);
        return (Mark[vertexEnd] == 2) ? Dist[vertexEnd] : notPath;
    }
    std::vector<Path> count(const TypeNameVer& vertex)
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
        bool operator()(const CurPath& a, const CurPath& b) const
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
    std::vector<Path> Dist;
    std::vector<int> Mark;
    std::set<CurPath, CmpPath> set;
    void begin(const TypeNameVer& vertex)
    {
        set.insert(std::make_pair(StayingPath(), vertex));
        Mark[vertex] = 1;
        while(1)
        {
            if(set.empty())
                break;
            CurPath minElem = *set.begin();
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
                shortestPath.Dist[vertex] = CalkPath()(shortestPath.Dist[root], weight);
                shortestPath.set.insert(std::make_pair(shortestPath.Dist[vertex], vertex));
            }
            else if(shortestPath.Mark[vertex] == 1 &&
              Cmp()(CalkPath()(shortestPath.Dist[root], weight), shortestPath.Dist[vertex]))
            {
                shortestPath.set.erase(std::make_pair(shortestPath.Dist[vertex], vertex));
                shortestPath.Dist[vertex] = CalkPath()(shortestPath.Dist[root], weight);
                shortestPath.set.insert(std::make_pair(shortestPath.Dist[vertex], vertex));
            }
        }
    };
};

#endif // SHORTESTPATH_H_INCLUDED
