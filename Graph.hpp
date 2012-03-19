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

#include "IncidenceType.hpp"
#include "Utils.hpp"

class Graph
{
 public:
  std::vector<VertexIncidenceType> vertexIncidents;

  bool isOutOfBound(int index) const
  {
    return (index < 0 || index >= vertexIncidents.size());
  }

  void addVertex(IncidenceType* incidence)
  {
    vertexIncidents.push_back(VertexIncidenceType(incidence));
  }
  
  template<typename IncidenceTypeT>
  void addVertex(int vertexNumber)
  {
    for(int i = 0; i < vertexNumber; i++)
      addVertex(new IncidenceTypeT);
  }

  void setVertexIncidenceType(int vertex_index, IncidenceType* new_incidence)
  {
    for(auto to : vertexIncidents[vertex_index])
    {
      new_incidence->addEdge(to);
    }
    vertexIncidents[vertex_index].clear();
    vertexIncidents[vertex_index] = std::move(VertexIncidenceType(new_incidence));
  }

  void addEdge(int source, int destination)
  {
    if (isOutOfBound(source) || isOutOfBound(destination))
      throw new std::out_of_range("Try to add bad edge from " + toString(source) + " " + toString(destination));

    vertexIncidents[source].addEdge(destination);
  }

  void removeEdge(int source, int destination)
  {
    if (isOutOfBound(source) || isOutOfBound(destination))
      throw new std::out_of_range("Try to remove bad edge from " + toString(source) + " " + toString(destination));

    vertexIncidents[source].removeEdge(destination);
  }

  bool hasEdge(int source, int destination) const
  {
    if (isOutOfBound(source) || isOutOfBound(destination))
      throw new std::out_of_range("Try to check bad edge from " + toString(source) + " " + toString(destination));

    return vertexIncidents[source].isConnectedTo(destination);
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
