/*
 * =====================================================================================
 *
 *       Filename:  GraphAlgorithms.hpp
 *
 *    Description:  GraphAlgorithms class stores basic algorithms for work with graphs.
 *
 *        Version:  1.0
 *        Created:  03/18/2012 04:47:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */
#ifndef GRAPHALGORITHMS_HPP
#define GRAPHALGORITHMS_HPP 

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <stack>
#include <queue>

#include <boost/optional.hpp>

#include "Graph.hpp"
#include "Edge.hpp"

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
      *path.push_back(parentEdge[destination]);
      destination = parentEdge[destination].source;
    }
    return path;
  }
 private:
  size_t source;
  std::vector< boost::optional<PathInfo> > distance;
  std::vector< boost::optional<EdgeType> > parentEdge;
}; // ShortestPathHolder

// Get rid of weight!
template <typename EdgeType,
          typename PathInfo,
          typename PathUpdater = std::plus<PathInfo>,
          typename PathComparator = std::less<PathInfo> >
class ShortestPathFinder
{
 public:
  explicit ShortestPathFinder(const Graph<EdgeType>& G): G(G) {}

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
    distance[source] = PathInfo();
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
        PathInfo newPath = updatePath(*distance[curV], edge.weight);
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

template <typename EdgeType,
          typename PathInfo,
          typename PathUpdater,
          typename PathComparator = std::less<PathInfo> >
class NaiveShortestPathFinder
{
 public:
  explicit NaiveShortestPathFinder(const Graph<EdgeType>& G): G(G) {}

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

class StronglyConnectedComponentsFinder
{
 public:
  explicit StronglyConnectedComponentsFinder(const Graph<BasicEdge>& G):G(G) {}

  std::vector<int> getStronglyConnectedComponents()
  {
    int vertexNumber = G.size();
    tin.assign(vertexNumber, 0);
    tup.assign(vertexNumber, 0);
    isInStack.assign(vertexNumber, 0);
    component.resize(vertexNumber);
    curTime = 1;
    componentCount = 0;
    for(int i = 0; i < vertexNumber; i++)
    {
      if (tin[i] == 0)
        dfs(i);
    }
    tin.clear();
    tup.clear();
    isInStack.clear();
    while (!S.empty())
      S.pop();

    return component;
  }

 private:
  const Graph<BasicEdge>& G;
  int curTime;
  int componentCount;
  std::stack<int> S;
  std::vector<int> tin;
  std::vector<int> tup;
  std::vector<int> component;
  std::vector<char> isInStack;

  void dfs(size_t v)
  {
    S.push(v);
    isInStack[v] = 1;
    tin[v] = tup[v] = curTime++;
    for(auto edge : G.vertexIncidents[v])
    {
      size_t to = edge.destination;
      if (tin[to] == 0)
      {
        dfs(to);
        tup[v] = std::min(tup[v], tup[to]);
      }
      else
        if (isInStack[to])
          tup[v] = std::min(tup[v], tin[to]);
    }
    if (tin[v] == tup[v])
    {
      int lastPop = -1;
      while (!S.empty() && lastPop != v)
      {
        component[S.top()] = componentCount;
        isInStack[S.top()] = 0;
        lastPop = S.top();
        S.pop();
      } 
      componentCount++;
    }
  }

}; // StronglyConnectedComponentsFinder

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

template< template<typename EdgeTypeT> class IncidenceTypeT = VectorIncidence >
Graph<BasicEdge> genRandomGraph(size_t vertexNumber, double edgeCreationProbability, size_t seed = 42)
{
  srand(seed);
  Graph<BasicEdge> G;
  G.addVerticies<IncidenceTypeT> (vertexNumber);

  for(int i = 0; i < vertexNumber; i++)
  {
    for(int j = 0; j < vertexNumber; j++)
    {
      double probability = rand() * 1.0 / RAND_MAX;
      if (edgeCreationProbability > probability)
      {
        G.vertexIncidents[i].addEdge(BasicEdge(i, j));
      }
    }
  }
  return G;
}

} // namespace graph_algorithms

#endif /* GRAPHALGORITHMS_HPP */