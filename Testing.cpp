/*
 * =====================================================================================
 *
 *       Filename:  Testing.cpp
 *
 *    Description:  Testing for GraphAlgorithms and Graph classes
 *
 *        Version:  1.0
 *        Created:  03/18/2012 07:41:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <unordered_set>
#include "Graph.hpp"
#include "GraphAlgorithms.hpp"
#include "Utils.hpp"

bool areEqual(const Graph& G1, const Graph& G2)
{
  int vertexNumber = G1.size();
  std::unordered_set<size_t> S;
  for(int i = 0; i < vertexNumber; i++)
  {
    for(auto to : G1.vertexIncidents[i])
      S.insert(to);
    for(auto to : G2.vertexIncidents[i])
      if (S.find(to) == S.end())
        return 0;
    S.clear();
  }
  return 1;
}

void testGraphClass(size_t size)
{
  Graph graph_with_vector_incidence  = graph_algorithms::getRandomGraph<VectorIncidence>  (size, 4.0 / size);
  Graph graph_with_set_incidence     = graph_algorithms::getRandomGraph<SetIncidence>     (size, 4.0 / size);
  Graph graph_with_bitmask_incidence = graph_algorithms::getRandomGraph<BitmaskIncidence> (size, 4.0 / size);

  if (!areEqual(graph_with_vector_incidence, graph_with_set_incidence))
    throw std::logic_error("Vector and Set based graphs are not equal. Graph size = " + toString(size));

  if (!areEqual(graph_with_vector_incidence, graph_with_bitmask_incidence))
    throw std::logic_error("Vector and Bitmask based graphs are not equal. Graph size = " + toString(size));

}

void testStronglyConnectedComponentsAlgorithm(size_t size)
{
  Graph graph = graph_algorithms::getRandomGraph<VectorIncidence> (size, 4.0 / size, size);
  graph_algorithms::StronglyConnectedComponentsFinder finder(graph);
  std::vector<int> component = finder.getStronglyConnectedComponents();
  int vertexNumber = graph.size();
  graph_algorithms::ConnectivityChecker checker(graph);
  for(int i = 0; i < vertexNumber; i++)
  {
    for(int j = 0; j < vertexNumber; j++)
    {
      if (component[i] == component[j])
      {
        if (!checker.checkConnectivity(i, j) || !checker.checkConnectivity(j, i))
          throw std::logic_error("Vertices " + toString(i) + " " + toString(j) +
                                 " are not connected. Graph size = " + toString(size));
      }
    }
  }
}

int main()
{
  {
    Timer timer("Testing Graph class");
    timer.start();
    testGraphClass(1e1);
    testGraphClass(1e2);
    testGraphClass(1e3);
    testGraphClass(1e4);
    timer.printTime();
    std::cout << "Graph class has passed tests" << std::endl;
  }
  {
    Timer timer("Testing Strongly Connected Components Finder");
    timer.start();
    testStronglyConnectedComponentsAlgorithm(1e1);
    testStronglyConnectedComponentsAlgorithm(1e2);
    testStronglyConnectedComponentsAlgorithm(5e2);
    timer.printTime();
    std::cout << "Strongly Connected Components Finder has passed tests" << std::endl;
  }
  return 0;
}
