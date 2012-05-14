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

#include "Graph.hpp"
#include "Edge.hpp"

namespace graph_algorithms
{

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
