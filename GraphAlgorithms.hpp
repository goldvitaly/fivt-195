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

class GraphAlgorithms
{
 public:
  static std::vector<int> getStronglyConnectedComponentsDivision(const Graph& G)
  {
    vertexNumber = G.size();
    tin.assign(vertexNumber, 0);
    tup.assign(vertexNumber, 0);
    isInStack.assign(vertexNumber, 0);
    component.resize(vertexNumber);
    curTime = 1;
    componentCount = 0;
    for(int i = 0; i < vertexNumber; i++)
    {
      if (tin[i] == 0)
        dfsForStronglyConnectedComponents(i, G);
    }
    tin.clear();
    tup.clear();
    isInStack.clear();
    while (!S.empty())
      S.pop();

    return std::move(component);
  }

  static bool checkConnectivity(int source, int destination, const Graph& G)
  {
    std::vector<size_t> dummyPath;
    return findPath(source, destination, dummyPath, G);
  }

  static bool findPath(int source, int destination, std::vector<size_t>& path_, const Graph& G)
  {
    visited.assign(G.size(), 0);
    bool pathExists = dfsForPathFinding(source, destination, G);
    visited.clear();
    if (pathExists)
    {
      path_ = std::move(path);
      reverse(path.begin(), path.end());
      return 1;
    }
    return 0;
  }

  template<typename IncidenceTypeT>
  static Graph getRandomGraph(int vertexNumber, double edgeCreationProbability, size_t seed = 42)
  {
    srand(seed);
    Graph G;
    G.addVertex<IncidenceTypeT> (vertexNumber);

    for(int i = 0; i < vertexNumber; i++)
    {
      for(int j = 0; j < vertexNumber; j++)
      {
        double probability = rand() * 1.0 / RAND_MAX;
        if (edgeCreationProbability > probability)
        {
          G.vertexIncidents[i].addEdge(j);
        }
      }
    }
    return G;
  }

 private:
  static std::vector<int> tin;
  static std::vector<int> tup;
  static std::vector<int> component; 
  static std::vector<bool> isInStack;
  static std::vector<bool> visited;
  static std::vector<size_t> path;
  static std::stack<int> S;
  static int vertexNumber;
  static int curTime;
  static int componentCount;

  static void dfsForStronglyConnectedComponents(int v, const Graph& G)
  {
    S.push(v);
    isInStack[v] = 1;
    tin[v] = tup[v] = curTime++;
    for(auto to : G.vertexIncidents[v])
    {
      if (tin[to] == 0)
      {
        dfsForStronglyConnectedComponents(to, G);
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

  static bool dfsForPathFinding(int v, int destination, const Graph& G)
  {
    visited[v] = 1;
    if (v == destination)
    {
      path.push_back(v);
      return 1;
    }
    for(auto to : G.vertexIncidents[v])
    {
      if (!visited[to])
      {
        if (dfsForPathFinding(to, destination, G))
        {
          path.push_back(v);
          return 1;
        }
      }
    }
    return 0;
  }

}; // GraphAlgorithms

std::vector<int> GraphAlgorithms::tin;
std::vector<int> GraphAlgorithms::tup;
std::vector<int> GraphAlgorithms::component;
std::vector<bool> GraphAlgorithms::isInStack;
std::vector<bool> GraphAlgorithms::visited;
std::vector<size_t> GraphAlgorithms::path;
std::stack<int> GraphAlgorithms::S;
int GraphAlgorithms::vertexNumber;
int GraphAlgorithms::curTime;
int GraphAlgorithms::componentCount;

#endif /* GRAPHALGORITHMS_HPP */
