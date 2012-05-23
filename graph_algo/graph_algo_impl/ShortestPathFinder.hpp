/*
 * =====================================================================================
 *
 *       Filename:  ShortestPathFinder.hpp
 *
 *    Description:  ShortestPathFinder class finds shortest path in graph
 *
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */

#ifndef SHORTESTPATHFINDER_HPP
#define SHORTESTPATHFINDER_HPP 

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>

#include <boost/optional.hpp>

#include "ShortestPathHolder.hpp"

namespace graph_algorithms
{

// Get rid of weight!
template <typename EdgeType,
          typename PathInfo,
          typename PathUpdater = std::plus<PathInfo>,
          typename PathComparator = std::less<PathInfo> >
class ShortestPathFinder
{
 public:
  explicit ShortestPathFinder(const Graph<EdgeType>& G, 
                              PathUpdater updatePath = PathUpdater(),
                              PathComparator comparePaths = PathComparator())
                              : G(G), updatePath(updatePath), comparePaths(comparePaths) {}

  void initShortestPathFinder()
  {
    distance.assign(G.size(), boost::none);
    parentEdge.assign(G.size(), boost::none);
    visited.assign(G.size(), 0);
  }

  void freeShortestPathFinder()
  {
    visited.clear();
  }

  ShortestPathHolder<PathInfo, EdgeType> findShortestPaths(size_t source)
  {
    if (source >= G.size())
      throw std::out_of_range("Try to find shortest path to wrong vertex " + toString(source));
    initShortestPathFinder();
    distance[source] = PathInfo(0);
    Q.push(queueState(*distance[source], source));
    while (!Q.empty()) 
    {
      queueState qTop = Q.top();
      Q.pop();
      size_t curV = qTop.vertexId;
      if (visited[curV])
        continue;
      visited[curV] = 1;
      for(auto edge : G.vertexIncidents[curV])
      {
        size_t nextV = edge.destination;
        PathInfo newPath = updatePath(*distance[curV], edge);
        if (!distance[nextV] || comparePaths(newPath, *distance[nextV]))
        {
          distance[nextV] = newPath;
          Q.push(queueState(*distance[nextV], nextV));
          parentEdge[nextV] = edge;
        }
      }
    }
    freeShortestPathFinder();
    return ShortestPathHolder<PathInfo, EdgeType> (source, std::move(distance), std::move(parentEdge));
  }

 private: 
  PathUpdater updatePath;
  PathComparator comparePaths;
  struct queueState
  {
    queueState(PathInfo pathInfo, size_t vertexId) : pathInfo(pathInfo), vertexId(vertexId) {}
    PathInfo pathInfo;
    size_t vertexId;
    bool operator <(const queueState& qState) const
    {
      if (PathComparator()(pathInfo, qState.pathInfo))
        return false;
      if (PathComparator()(qState.pathInfo, pathInfo))
        return true;
      return vertexId < qState.vertexId;
    }
  };
  const Graph<EdgeType>& G;
  std::vector< boost::optional<PathInfo> > distance;
  std::vector< boost::optional<EdgeType> > parentEdge;
  std::vector<char> visited;
  std::priority_queue<queueState> Q;

}; // ShortestPathFinder

} // namespace graph_algorithms

#endif /* SHORTESTPATHFINDER_HPP */