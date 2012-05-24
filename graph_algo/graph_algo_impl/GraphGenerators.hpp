/*
 * =====================================================================================
 *
 *       Filename:  GraphGenerators.hpp
 *
 *    Description:  Stores difference graph generators
 *
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */
#ifndef GRAPHGENERATORS_HPP
#define GRAPHGENERATORS_HPP 

#include <cstdlib>
#include <cstdio>

#include "../../graph/Graph.hpp"
#include "../../edge/Edge.hpp"
#include "../../graph/incidence/incidence_impl/VectorIncidence.hpp"

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

#endif /* GRAPHGENERATORS_HPP */
