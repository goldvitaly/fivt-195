/*
 * =====================================================================================
 *
 *       Filename:  ConnectivityChecker.hpp
 *
 *    Description:  ConnectivityChecker class checks connectivity between two verticies in graph
 *
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */

#ifndef CONNECTIVITYCHECKER_HPP
#define CONNECTIVITYCHECKER_HPP 

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <vector>

#include <boost/optional.hpp>

namespace graph_algorithms
{

class ConnectivityChecker
{
 public:
  explicit ConnectivityChecker(const Graph<BasicEdge>& G):G(G) {}

  bool checkConnectivity(size_t source, size_t destination)
  {
    visited.assign(G.size(), 0);
    bool result = dfs(source, destination);
    visited.clear();
    return result;
  }

 private:
  const Graph<BasicEdge>& G;
  std::vector<char> visited;

  bool dfs(size_t v, size_t destination)
  {
    visited[v] = 1;
    if (v == destination)
      return true;
    for(auto edge : G.vertexIncidents[v])
    {
      size_t to = edge.destination;
      if (!visited[to])
      {
        if (dfs(to, destination))
          return true;
      }
    }
    return false;
  }
  
}; // ConnectivityChecker

} // namespace graph_algorithms

#endif /* CONNECTIVITYCHECKER_HPP */