/*
 * =====================================================================================
 *
 *       Filename:  ShortestPathHolder.hpp
 *
 *    Description:  ShortestPathHolder class stores path computed by ShortestPathFinder
 *
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */

#ifndef SHORTESTPATHHOLDER_HPP
#define SHORTESTPATHHOLDER_HPP 

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <vector>

#include <boost/optional.hpp>

namespace graph_algorithms
{

template
<typename PathInfo,
 typename EdgeType>
class ShortestPathHolder
{
 public:
  ShortestPathHolder() {}
  explicit ShortestPathHolder(size_t source,
                              const std::vector< boost::optional<PathInfo> >& distance,
                              const std::vector< boost::optional<EdgeType> >& parentEdge) 
                              : source(source),
                                distance(distance),
                                parentEdge(parentEdge) {}

  boost::optional<PathInfo> getDistance(size_t destination) const
  {
    if (destination >= distance.size())
      throw std::out_of_range("Try to get distance to wrong vertex " + toString(destination));
    return distance[destination];
  }
  boost::optional< std::vector<EdgeType> > getPath(size_t destination) const
  {
    if (destination >= distance.size())
      throw std::out_of_range("Try to get path to wrong vertex " + toString(destination));
    boost::optional< std::vector<EdgeType> > path;
    while (parentEdge[destination])
    {
      (*path).push_back(*parentEdge[destination]);
      destination = parentEdge[destination]->source;
    }
    return path;
  }
 private:
  size_t source;
  std::vector< boost::optional<PathInfo> > distance;
  std::vector< boost::optional<EdgeType> > parentEdge;
}; // ShortestPathHolder

} // namespace graph_algorithms

#endif /* SHORTESTPATHHOLDER_HPP */