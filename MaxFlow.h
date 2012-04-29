#ifndef MAXFLOW_H_INCLUDED
#define MAXFLOW_H_INCLUDED

#include <map>
#include <utility>
#include <algorithm>

#include "graph.h"
#include "ShortestPath.h"

template<class Weight>
class CalkFlow
{
    typedef unsigned int TypeNameVer;
    typedef std::map<std::pair<TypeNameVer, TypeNameVer>, Weight> mapEdge;
    mapEdge& flow;
public:
    explicit CalkFlow(mapEdge& flow_) : flow(flow_)
    {

    }
    Weight operator()(const AccessPath<Weight, Weight>& accessPath)
    {
        TypeNameVer verOut, verIn;
        verIn = accessPath.lastVertex();
        verOut = accessPath.prev()->lastVertex();
        Weight flowEdge =  flow[std::pair<TypeNameVer, TypeNameVer>(verOut, verIn)];
        Weight weightEdge = accessPath.weight() - flowEdge;
        if(accessPath.prev()->prev() == NULL)
            return weightEdge;
        else
            return std::min(weightEdge, accessPath.path());
    }
};

template<class Weight>
class CmpEdge
{
public:
    bool operator()(const Weight& f, const Weight& s) const
    {
        return f > s;
    }
};

template<class StructVer, class Weight>
class MaxFlow
{
typedef unsigned int TypeNameVer;
typedef std::map<std::pair<TypeNameVer, TypeNameVer>, Weight> mapEdge;
public:
    explicit MaxFlow(const Graph<StructVer, Weight>& graph_) : graph(graph_)
    {

    }
    Weight count(const TypeNameVer& source_, const TypeNameVer& target_)
    {
        source = source_;
        target = target_;
        flow.clear();
        return begin();
    }
private:
    const Graph<StructVer, Weight>& graph;
    TypeNameVer source, target;
    mapEdge flow;
    Weight begin()
    {
        Weight AnsFlow = 0;
        Weight delFlow = 1;
        while(delFlow > 0)
        {
            ShortestPath<StructVer, Weight, Weight, CalkFlow<Weight>, CmpEdge<Weight> > shortestPath(graph, CalkFlow<Weight>(flow));
            delFlow = shortestPath.count(source, target, -1);
            AnsFlow += delFlow;

            AccessPath<Weight, Weight> accessPath = shortestPath.path(target);
            while(accessPath.prev() != NULL)
            {
                flow[std::make_pair(accessPath.prev()->lastVertex(), accessPath.lastVertex())] += delFlow;
                flow[std::make_pair(accessPath.lastVertex(), accessPath.prev()->lastVertex())] -= delFlow;
                accessPath = *accessPath.prev();
            }

            std::cout << "delFlow = " << delFlow << std::endl;
        }
        return AnsFlow;
    }
};


#endif // MAXFLOW_H_INCLUDED
