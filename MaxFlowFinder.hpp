/*
 * =====================================================================================
 *
 *       Filename:  MaxFlowFinder.hpp
 *
 *    Description:  MaxFlowFinder class finds max flow in graph
 *
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */

#ifndef MAXFLOWFINDER_HPP
#define MAXFLOWFINDER_HPP 

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <vector>

#include <boost/optional.hpp>

namespace graph_algorithms
{

template<typename FlowType>
struct MinEdgeInPath
{
  FlowType operator() (const FlowType& currentEdge, const FlowType& currentMinEdgeInPath) const
  {
    return std::min(currentEdge, currentMinEdgeInPath);
  }
};

template<typename EdgeType, typename FlowType>
class MaxFlowFinder
{
 public:
  explicit MaxFlowFinder(Graph<EdgeType>& G) : G(G) 
  {
    for(int v = 0; v < G.size(); ++v)
    {
      for(const auto& edge : G.vertexIncidents[v])
      {
        graphEdge[edge.index] = edge;
      }
    }
  }

  FlowType calculateMaxFlow(size_t source, size_t sink)
  {
    ShortestPathFinder<EdgeType, FlowType, MinEdgeInPath<FlowType>, std::greater<FlowType> > shortestPathFinder(G);
    FlowType flow = FlowType(0);
    while (true)
    {
      auto shortestPathHolder = shortestPathFinder.findShortestPaths(source);
      boost::optional<FlowType> distance = shortestPathHolder.getDistance(sink);
      if ((!distance) || (*distance == FlowType(0)))
        break;
      FlowType addFlow = *distance;

      std::vector<EdgeType> shortestPath = *shortestPathHolder.getPath(sink);
      for(auto edge : shortestPath)
      {
        graphEdge[edge.index]->flow += addFlow;
        graphEdge[edge.backEdgeIndex]->flow -= addFlow;
      }
      flow += addFlow;
    }
    return flow;
  }

 private:
  Graph<EdgeType>& G;
  std::unordered_map<size_t, EdgeType*> graphEdge;
}; // MaxFlowFinder

} // namespace graph_algorithms

#endif /* MAXFLOWFINDER_HPP */