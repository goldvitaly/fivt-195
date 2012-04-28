#ifndef SHORTESTPATH_H_INCLUDED
#define SHORTESTPATH_H_INCLUDED

#include "graph.h"
#include <map>
#include <vector>
#include <iostream>

template<class Weight, class Path>
class AccessPath
{
    typedef unsigned int TypeNameVer;
    Weight weightEdge;
    Path pathPrevVertex;
    TypeNameVer vertex;
    AccessPath* prevAccessPath;
public:
    AccessPath() {
        vertex = 0;
        prevAccessPath = NULL;
    }
    AccessPath(const TypeNameVer& nameVer, AccessPath* accessPath_, Weight weight_, Path path_) {
        vertex = nameVer;
        weightEdge = weight_;
        pathPrevVertex = path_;
        prevAccessPath = accessPath_;
    }
    TypeNameVer lastVertex() const {
        return vertex;
    }
    AccessPath* prev() const {
        return prevAccessPath;
    }
    Weight weight() const {
        return weightEdge;
    }
    Path path() const {
        return pathPrevVertex;
    }
};


template<class StructVer, class Weight, class Path, class CalkPath, class Cmp = std::less<Path>,
         class StayingPath = Path>
class ShortestPath
{
typedef unsigned int TypeNameVer;
typedef typename std::set<std::pair<Path, TypeNameVer> >::iterator SetIterator;
typedef std::pair<Path, TypeNameVer> CurPath;
public:
    explicit ShortestPath(const Graph<StructVer, Weight>& graph_) : graph(graph_)
    {

    }
    Path count(const TypeNameVer& vertexStart, const TypeNameVer& vertexEnd, const Path& notPath)
    {
        init(notPath);
        begin(vertexStart);
        return (mark[vertexEnd] == 2) ? dist[vertexEnd] : notPath;
    }
    std::vector<Path> count(const TypeNameVer& vertex, const Path& notPath)
    {
        init(notPath);
        begin(vertex);
        return dist;
    }
    AccessPath<Weight, Path> path(const TypeNameVer& vertex) const
    {
        return accessPath[vertex];
    }
private:
    void init(const Path& notPath)
    {
        mark.clear();
        accessPath.clear();
        dist.clear();

        mark.resize(graph.size());
        accessPath.resize(graph.size());
        dist.resize(graph.size(), notPath);
    }

    class CmpPath{
    public:
        bool operator()(const CurPath& a, const CurPath& b) const
        {
            if(Cmp()(a.first, b.first))
                return true;
            else if(Cmp()(a.first, b.first) && Cmp()(b.first, a.first) && a.second < b.second)
                return true;
            else
                return false;
        }
    };

    const Graph<StructVer, Weight>& graph;
    std::vector<AccessPath<Weight, Path> > accessPath;
    std::vector<Path> dist;
    std::vector<int> mark;
    std::set<CurPath, CmpPath> set;

    void begin(const TypeNameVer& vertex)
    {
        dist[vertex] = StayingPath();
        set.insert(std::make_pair(StayingPath(), vertex));
        accessPath[vertex] = AccessPath<Weight, Path>(vertex, 0, Weight(), StayingPath());
        mark[vertex] = 1;
        while(1)
        {
            if(set.empty())
                break;
            CurPath minElem = *set.begin();
            mark[minElem.second] = 2;
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
            AccessPath<Weight, Path> newAccessPath =
                AccessPath<Weight, Path>(vertex, &shortestPath.accessPath[root], weight, shortestPath.dist[root]);
            Path newPath = CalkPath()(newAccessPath);
            if(shortestPath.mark[vertex] == 0)
            {
                shortestPath.mark[vertex] = 1;
                shortestPath.accessPath[vertex] = newAccessPath;
                shortestPath.dist[vertex] = newPath;
                shortestPath.set.insert(std::make_pair(shortestPath.dist[vertex], vertex));
            }
            else if(shortestPath.mark[vertex] == 1 && Cmp()(newPath, shortestPath.dist[vertex]))
            {
                shortestPath.set.erase(std::make_pair(shortestPath.dist[vertex], vertex));
                shortestPath.dist[vertex] = newPath;
                shortestPath.accessPath[vertex] = newAccessPath;
                shortestPath.set.insert(std::make_pair(shortestPath.dist[vertex], vertex));
            }
        }
    };
};

#endif // SHORTESTPATH_H_INCLUDED
