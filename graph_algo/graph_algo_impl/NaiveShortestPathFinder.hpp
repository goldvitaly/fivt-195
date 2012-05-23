/*
 * =====================================================================================
 *
 *       Filename:  NaiveShortestPathFinder.hpp
 *
 *    Description:  NaiveShortestPathFinder class finds shortest path in graph
 *
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */

#ifndef NAIVESHORTESTPATHFINDER_HPP
#define NAIVESHORTESTPATHFINDER_HPP 

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <vector>

#include <boost/optional.hpp>

#include "ShortestPathHolder.hpp"

namespace graph_algorithms
{

template <typename EdgeType,
          typename PathInfo,
          typename PathUpdater,
          typename PathComparator = std::less<PathInfo> >
class NaiveShortestPathFinder
{
 public:
  explicit NaiveShortestPathFinder(const Graph<EdgeType>& G, 
                                    PathUpdater updatePath = PathUpdater(),
                                    PathComparator comparePaths = PathComparator())
                                    : G(G), updatePath(updatePath), comparePaths(comparePaths) {}

  void initNaiveShortestPathFinder()
  {
    distance.assign(G.size(), boost::none);
    parentEdge.assign(G.size(), boost::none);
  }

  ShortestPathHolder<PathInfo, EdgeType> findShortestPaths(size_t source)
  {
    if (source >= G.size())
      throw std::out_of_range("Try to find shortest path to wrong vertex " + toString(source));
    initNaiveShortestPathFinder();
    distance[source] = PathInfo();
    for(size_t iteration = 0; iteration < G.size(); iteration++)
    {
      for(size_t vertex = 0; vertex < G.size(); vertex++)
      {
        for(auto edge : G.vertexIncidents[vertex])
        {
          if (!distance[edge.source])
            continue;
          PathInfo newPath = updatePath(*distance[edge.source], edge.weight);
          if (!distance[edge.destination] || comparePaths(newPath, *distance[edge.destination]))
          {
            distance[edge.destination] = newPath;
            parentEdge[edge.destination] = edge;
          }
        }
      }
    }
    return ShortestPathHolder<PathInfo, EdgeType> (source, std::move(distance), std::move(parentEdge));
  }


 private:
  const Graph<EdgeType>& G;
  std::vector< boost::optional<PathInfo> > distance;
  std::vector< boost::optional<EdgeType> > parentEdge;
  PathUpdater updatePath;
  PathComparator comparePaths;
}; // NaiveShortestPathFinder

} // namespace graph_algorithms

#endif /* NAIVESHORTESTPATHFINDER_HPP */