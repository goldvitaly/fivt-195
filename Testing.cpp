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
#include "Edge.hpp"
#include "Utils.hpp"

template<typename EdgeType>
bool areEqual(const Graph<EdgeType>& G1, const Graph<EdgeType>& G2)
{
  int vertexNumber = G1.size();
  std::vector<EdgeType> V;
  for(int i = 0; i < vertexNumber; i++)
  {
    for(auto edge : G1.vertexIncidents[i])
      V.push_back(edge);
    for(auto edge : G2.vertexIncidents[i])
      if (std::find(V.begin(), V.end(), edge) == V.end())
        return false;
    V.clear();
  }
  return true;
}

void testGraphClass(size_t size)
{
  Graph<BasicEdge> graph_with_vector_incidence  = graph_algorithms::getRandomGraph<VectorIncidence>  (size, 4.0 / size);
  Graph<BasicEdge> graph_with_set_incidence     = graph_algorithms::getRandomGraph<SetIncidence>     (size, 4.0 / size);
  Graph<BasicEdge> graph_with_bitmask_incidence = graph_algorithms::getRandomGraph<BitmaskIncidence> (size, 4.0 / size);

  if (!areEqual(graph_with_vector_incidence, graph_with_set_incidence))
    throw std::logic_error("Vector and Set based graphs are not equal. Graph size = " + toString(size));

  if (!areEqual(graph_with_vector_incidence, graph_with_bitmask_incidence))
    throw std::logic_error("Vector and Bitmask based graphs are not equal. Graph size = " + toString(size));

}

void testStronglyConnectedComponentsAlgorithm(size_t size)
{
  Graph<BasicEdge> graph = graph_algorithms::getRandomGraph<VectorIncidence> (size, 4.0 / size, size);
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

template<typename T>
struct Plus
{
  const T& operator() (const T& a, const T& b) const
  {
    return a + b;
  }
};

template<typename T>
struct Less
{
  bool operator() (const T& a, const T& b) const
  {
    return a < b;
  }
};

void testShortestPathAlgorithm(size_t size)
{
  Graph< WeightedEdge<int> > graph;
  graph_algorithms::ShortestPathFinder< int, int, Plus<int>, Less<int> > finder(graph);
}

int main()
{
  int vertexNumber = 10;
  Graph<BasicEdge> G;
  G.addVerticies<BitmaskIncidence>(vertexNumber);
  for(int i = 0; i < 10; i++)
    G.addEdge(BasicEdge(rand() % vertexNumber, rand() % vertexNumber));

  for(int v = 0; v < vertexNumber; v++)
  {
    for(auto edge : G.vertexIncidents[v])
    {
      std::cout << "Edge from " << edge.source << " to " << edge.destination << std::endl;
    }
  }
  testShortestPathAlgorithm(10);
  /*
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
    timer.printTime();
    std::cout << "Strongly Connected Components Finder has passed tests" << std::endl;
  }
  */
  return 0;
}
