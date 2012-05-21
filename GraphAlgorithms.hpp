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

#include <boost/optional.hpp>

#include "Graph.hpp"
#include "Edge.hpp"

namespace graph_algorithms
{

// Get rid of weight!
template <typename Weight,
          typename PathInfo,
          typename PathUpdater,
          typename PathComparator = std::less<PathInfo> >
class ShortestPathFinder
{
 public:
  explicit ShortestPathFinder(const Graph< WeightedEdge<Weight> >& G): G(G) {}

  void initShortestPathFinder()
  {
    previousVertex.resize(G.size());
    distance.resize(G.size());
    visited.assign(G.size(), 0);
  }

  void freeShortestPathFinder()
  {
    previousVertex.clear();
    distance.clear();
    visited.clear();
  }

  std::vector<size_t> findShortestPathToSingleVertex(size_t source, size_t destination)
  {
    initShortestPathFinder();
    std::vector<size_t> shortestPath;
    distance[source] = 0;
    while (1) 
    {
      boost::optional<size_t> curV;
      for(int i = 0; i < G.size(); ++i)
      {
        if (visited[i] || !distance[i])
          continue;
        if (!curV || (cmp(distance[curV], distance[i])))
          curV = i;
      }
      if (!curV)
        break;
      visited[*curV] = 1;
      for(auto edge : G.vertexIncidents[curV])
      {
        int nextV = edge.destination;
        PathInfo newPath = upd(distance[curV], edge);
        if (!distance[nextV] || cmp(newPath, *distance[nextV]))
        {
          *distance[nextV] = newPath;
        }
      }
    }
    freeShortestPathFinder();
    return shortestPath;
  }

 private: 
  const Graph< WeightedEdge<Weight> >& G;
  std::vector< boost::optional<size_t> > previousVertex;
  std::vector< boost::optional<PathInfo> > distance;
  std::vector<char> visited;
  PathUpdater upd;
  PathComparator cmp;

};

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
Graph<BasicEdge> getRandomGraph(size_t vertexNumber, double edgeCreationProbability, size_t seed = 42)
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

/*
template< template<typename EdgeTypeT> class IncidenceTypeT = VectorIncidence, typename EdgeType >
Graph<EdgeType> getRandomGraph(size_t vertexNumber, double edgeCreationProbability, size_t seed = 42)
{
  srand(seed);
  Graph<EdgeType> G;
  G.addVertices< IncidenceTypeT<EdgeType> > (vertexNumber);

  for(int i = 0; i < vertexNumber; i++)
  {
    for(int j = 0; j < vertexNumber; j++)
    {
      double probability = rand() * 1.0 / RAND_MAX;
      if (edgeCreationProbability > probability)
      {
        G.vertexIncidents[i].addEdge(EdgeType(i, j));
      }
    }
  }
  return G;
}
*/

} // namespace graph_algorithms

#endif /* GRAPHALGORITHMS_HPP */