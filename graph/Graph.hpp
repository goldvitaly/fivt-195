/*
 * =====================================================================================
 *
 *       Filename:  Graph.hpp
 *
 *    Description:  Graph class description
 *
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */
#ifndef GRAPH_HPP
#define GRAPH_HPP 

#include <vector>
#include <stdexcept>

#include "incidence/VertexIncidence.hpp"
#include "../edge/Edge.hpp"
#include "../utils/Utils.hpp"

template<typename EdgeType>
class Graph
{
 public:
  std::vector< VertexIncidenceType<EdgeType> > vertexIncidents;
  size_t edgeNumber;

  bool isOutOfBound(size_t index) const
  {
    return (index >= vertexIncidents.size());
  }

  void addVertex(IncidenceType<EdgeType>* incidence)
  {
    vertexIncidents.push_back(VertexIncidenceType<EdgeType>(incidence));
  }
  
  template< template<typename EdgeTypeT> class IncidenceTypeT >
  void addVerticies(int vertexNumber)
  {
    for(int i = 0; i < vertexNumber; i++)
      addVertex(new IncidenceTypeT<EdgeType>);
  }

  void setVertexIncidenceType(size_t vertex, IncidenceType<EdgeType>* new_incidence)
  {
    if (isOutOfBound(vertex))
      throw new std::out_of_range("Try to change wrong vertex " + toString(vertex));

    for(auto edge : vertexIncidents[vertex])
    {
      new_incidence->addEdge(edge);
    }
    vertexIncidents[vertex].clear();
    vertexIncidents[vertex] = std::move(VertexIncidenceType<EdgeType>(new_incidence));
  }

  void addEdge(EdgeType edge)
  {
    if (isOutOfBound(edge.source) || isOutOfBound(edge.destination))
      throw new std::out_of_range("Try to add bad edge from " 
                                  + toString(edge.source) + " " 
                                  + toString(edge.destination));

    edgeNumber++;
    vertexIncidents[edge.source].addEdge(edge);
  }

  void removeEdge(EdgeType edge)
  {
    if (isOutOfBound(edge.source) || isOutOfBound(edge.destination))
      throw new std::out_of_range("Try to remove bad edge from " 
                                  + toString(edge.source) + " " 
                                  + toString(edge.destination));

    vertexIncidents[edge.source].removeEdge(edge);
  }

  bool hasEdge(EdgeType edge) const
  {
    if (isOutOfBound(edge.source) || isOutOfBound(edge.destination))
      throw new std::out_of_range("Try to check bad edge from " 
                                  + toString(edge.destination) + " " 
                                  + toString(edge.destination));

    return vertexIncidents[edge.source].hasEdge(edge);
  }

  size_t size() const
  {
    return vertexIncidents.size();
  }

  void clear()
  {
    for(auto& v : vertexIncidents)
      v.clear();
    vertexIncidents.clear();
  }

  ~Graph()
  {
    clear();
  }

}; // Graph

#endif /* GRAPH_HPP */
