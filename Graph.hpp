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

  void addEdge(int source, int destination)
  {
    if (isOutOfBound(source) || isOutOfBound(destination))
      throw new std::exception;

    vertexIncidents[source].addEdge(destination);
  }

  void removeEdge(int source, int destination)
  {
    if (isOutOfBound(source) || isOutOfBound(destination))
      throw new std::exception;

    vertexIncidents[source].removeEdge(destination);
  }

  bool hasEdge(int source, int destination) const
  {
    if (isOutOfBound(source) || isOutOfBound(destination))
      throw new std::exception;

    return vertexIncidents[source].isConnectedTo(destination);
  }

  size_t size() const
  {
    return vertexIncidents.size();
  }

  void clear()
  {
    for(int i = 0; i < vertexIncidents.size(); i++)
    {
      vertexIncidents[i].clear();
    }
    vertexIncidents.clear();
  }

  ~Graph()
  {
    clear();
  }

}; // Graph

#endif /* GRAPH_HPP */
