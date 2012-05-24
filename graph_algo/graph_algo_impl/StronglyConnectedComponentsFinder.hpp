/*
 * =====================================================================================
 *
 *       Filename:  StronglyConnectedComponentsFinder.hpp
 *
 *    Description:  StronglyConnectedComponentsFinder class finds strongly connected components in graph
 *
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */

#ifndef STRONGLYCONNECTEDCOMPONENTSFINDER_HPP
#define STRONGLYCONNECTEDCOMPONENTSFINDER_HPP 

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <stack>

#include <boost/optional.hpp>

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

} // namespace graph_algorithms

#endif /* STRONGLYCONNECTEDCOMPONENTSFINDER_HPP */