#ifndef SHORTESTPATH_H_INCLUDED
#define SHORTESTPATH_H_INCLUDED

#include "graph.h"
#include <map>
#include <vector>
#include <iostream>

template<class Weight, class LengthPath>
class AccessPath
{
    typedef unsigned int TypeNameVer;
    Weight weightEdge;
    LengthPath pathPrevVertex;
    TypeNameVer vertex;
    AccessPath* prevAccessPath;
public:
    AccessPath() {
        vertex = 0;
        prevAccessPath = NULL;
    }
    AccessPath(const TypeNameVer& nameVer, AccessPath* accessPath_, Weight weight_, LengthPath lengthPath_) {
        vertex = nameVer;
        weightEdge = weight_;
        pathPrevVertex = lengthPath_;
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
    LengthPath path() const {
        return pathPrevVertex;
    }
};


template<class StructVer, class Weight, class LenghtPath, class CalculatePath, class Cmp = std::less<LenghtPath>,
         class StayingPath = LenghtPath>
class ShortestPath
{
typedef unsigned int TypeNameVer;
typedef typename std::set<std::pair<LenghtPath, TypeNameVer> >::iterator SetIterator;
typedef std::pair<LenghtPath, TypeNameVer> CurLenghtPath;
public:
    explicit ShortestPath(const Graph<StructVer, Weight>& graph_, CalculatePath calculatePath_ = CalculatePath())
        : graph(graph_), calculatePath(calculatePath_)
    {

    }
    LenghtPath count(const TypeNameVer& vertexStart, const TypeNameVer& vertexEnd, const LenghtPath& notExistPath)
    {
        init(notExistPath);
        begin(vertexStart);
        return (mark[vertexEnd] == 2) ? dist[vertexEnd] : notExistPath;
    }
    std::vector<LenghtPath> count(const TypeNameVer& vertex, const LenghtPath& notExistPath)
    {
        init(notExistPath);
        begin(vertex);
        return dist;
    }
    AccessPath<Weight, LenghtPath> path(const TypeNameVer& vertex) const
    {
        return accessPath[vertex];
    }
private:
    void init(const LenghtPath& notExistPath)
    {
        mark.clear();
        accessPath.clear();
        dist.clear();

        mark.resize(graph.size());
        accessPath.resize(graph.size());
        dist.resize(graph.size(), notExistPath);
    }

    class CmpPath{
    public:
        bool operator()(const CurLenghtPath& a, const CurLenghtPath& b) const
        {
            if(Cmp()(a.first, b.first))
                return true;
            else if(!Cmp()(b.first, a.first) && a.second < b.second)
                return true;
            else
                return false;
        }
    };

    const Graph<StructVer, Weight>& graph;
    std::vector<AccessPath<Weight, LenghtPath> > accessPath;
    std::vector<LenghtPath> dist;
    std::vector<int> mark;
    std::set<CurLenghtPath, CmpPath> setGreyVertex;
    CalculatePath calculatePath;

    void begin(const TypeNameVer& vertex)
    {
        dist[vertex] = StayingPath();
        setGreyVertex.insert(std::make_pair(StayingPath(), vertex));
        accessPath[vertex] = AccessPath<Weight, LenghtPath>(vertex, 0, Weight(), StayingPath());
        mark[vertex] = 1;

        while(!setGreyVertex.empty())
        {
            CurLenghtPath minElem = *setGreyVertex.begin();
            mark[minElem.second] = 2;
            setGreyVertex.erase(setGreyVertex.begin());
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
            AccessPath<Weight, LenghtPath> newAccessPath =
                AccessPath<Weight, LenghtPath>(vertex, &shortestPath.accessPath[root], weight, shortestPath.dist[root]);
            LenghtPath newPath = shortestPath.calculatePath(newAccessPath);
            if(shortestPath.mark[vertex] == 0
               || (shortestPath.mark[vertex] == 1 && Cmp()(newPath, shortestPath.dist[vertex])))
            {
                if(shortestPath.mark[vertex] == 1)
                    shortestPath.setGreyVertex.erase(std::make_pair(shortestPath.dist[vertex], vertex));
                else
                    shortestPath.mark[vertex] = 1;

                shortestPath.dist[vertex] = newPath;
                shortestPath.accessPath[vertex] = newAccessPath;
                shortestPath.setGreyVertex.insert(std::make_pair(shortestPath.dist[vertex], vertex));
            }
        }
    };
};

#endif // SHORTESTPATH_H_INCLUDED
