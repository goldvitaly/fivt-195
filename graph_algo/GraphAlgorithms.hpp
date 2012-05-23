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
#include <unordered_map>

#include <boost/optional.hpp>

#include "Graph.hpp"
#include "Edge.hpp"

namespace graph_algorithms
{

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
