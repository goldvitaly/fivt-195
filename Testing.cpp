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
  Graph<BasicEdge> graph_with_vector_incidence  = graph_algorithms::genRandomGraph<VectorIncidence>  (size, 4.0 / size);
  Graph<BasicEdge> graph_with_set_incidence     = graph_algorithms::genRandomGraph<SetIncidence>     (size, 4.0 / size);
  Graph<BasicEdge> graph_with_bitmask_incidence = graph_algorithms::genRandomGraph<BitmaskIncidence> (size, 4.0 / size);

  if (!areEqual(graph_with_vector_incidence, graph_with_set_incidence))
    throw std::logic_error("Vector and Set based graphs are not equal. Graph size = " + toString(size));

  if (!areEqual(graph_with_vector_incidence, graph_with_bitmask_incidence))
    throw std::logic_error("Vector and Bitmask based graphs are not equal. Graph size = " + toString(size));

}

void testStronglyConnectedComponentsAlgorithm(size_t size)
{
  Graph<BasicEdge> graph = graph_algorithms::genRandomGraph<VectorIncidence> (size, 4.0 / size, size);
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

void testShortestPathAlgorithm(size_t vertexNumber)
{
  Graph< WeightedEdge<int> > graph;
  graph.addVerticies<VectorIncidence>(vertexNumber);
  for(int i = 0; i < vertexNumber * log(vertexNumber); i++)
    graph.addEdge(WeightedEdge<int>(rand() % vertexNumber, rand() % vertexNumber, rand() % 10000));

  graph_algorithms::ShortestPathFinder< WeightedEdge<int>, int, std::plus<int>, std::less<int> > finder(graph);
  graph_algorithms::ShortestPathHolder< int, WeightedEdge<int> > pathInfo = finder.findShortestPaths(0);

  graph_algorithms::NaiveShortestPathFinder< WeightedEdge<int>, int, std::plus<int>, std::less<int> > naiveFinder(graph);
  graph_algorithms::ShortestPathHolder< int, WeightedEdge<int> > naivePathInfo = naiveFinder.findShortestPaths(0);

  for(int v = 0; v < vertexNumber; v++)
  {
    boost::optional<int> dist = pathInfo.getDistance(v);
    boost::optional<int> naiveDist = naivePathInfo.getDistance(v);
    if (dist != naiveDist)
    {
      if (naiveDist)
        throw std::logic_error("ShortestPathFinder missed path to " + toString(v));
      else
        throw std::logic_error("ShortestPathFinder found wrong path to " + toString(v));
    }
    if (dist && (*dist != *naiveDist))
      throw std::logic_error("Wrong distance to " + toString(v));
  }
}

template<typename FlowType>
void addFlowEdge(Graph< FlowEdge<FlowType> >& G, size_t source, size_t destination, 
                 FlowType capacity, size_t index1, size_t index2)
{
  G.addEdge(FlowEdge<FlowType>(source, destination, capacity, FlowType(0), index1, index2));
  G.addEdge(FlowEdge<FlowType>(destination, source, FlowType(0), FlowType(0), index2, index1));
}

int main()
{
  int vertexNumber = 10;
  Graph< FlowEdge<int> > G;
  G.addVerticies<VectorIncidence>(vertexNumber);
  for(int i = 0; i < 10; i++)
  {
    addFlowEdge(G, rand() % vertexNumber, rand()%vertexNumber, rand()%1000, 2*i, 2*i + 1);
  }
  graph_algorithms::MaxFlowFinder<FlowEdge<int>, int> maxFlowFinder(G);
  std::cout << maxFlowFinder.calculateMaxFlow(0, 10) << std::endl;
  {
    Timer timer("Testing Shortest Path Finder");
    timer.start();
    testShortestPathAlgorithm(1e1);
    testShortestPathAlgorithm(1e2);
    testShortestPathAlgorithm(1e3);
    timer.printTime();
    std::cout << "Shortest Path Finder has passed tests" << std::endl;
  }
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
  return 0;
}
