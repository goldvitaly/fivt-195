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

  bool isOutOfBound(size_t index) const
  {
    return (index >= vertexIncidents.size());
  }

  void addVertex(IncidenceType* incidence)
  {
    vertexIncidents.push_back(VertexIncidenceType(incidence));
  }
  
  template<typename IncidenceTypeT>
  void addVertices(int vertexNumber)
  {
    for(int i = 0; i < vertexNumber; i++)
      addVertex(new IncidenceTypeT);
  }

  void setVertexIncidenceType(size_t vertex, IncidenceType* new_incidence)
  {
    if (isOutOfBound(vertex))
      throw new std::out_of_range("Try to change wrong vertex " + toString(vertex));

    for(auto to : vertexIncidents[vertex])
    {
      new_incidence->addEdge(to);
    }
    vertexIncidents[vertex].clear();
    vertexIncidents[vertex] = std::move(VertexIncidenceType(new_incidence));
  }

  void addEdge(size_t source, size_t destination)
  {
    if (isOutOfBound(source) || isOutOfBound(destination))
      throw new std::out_of_range("Try to add bad edge from " + toString(source) + " " + toString(destination));

    vertexIncidents[source].addEdge(destination);
  }

  void removeEdge(size_t source, size_t destination)
  {
    if (isOutOfBound(source) || isOutOfBound(destination))
      throw new std::out_of_range("Try to remove bad edge from " + toString(source) + " " + toString(destination));

    vertexIncidents[source].removeEdge(destination);
  }

  bool hasEdge(size_t source, size_t destination) const
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

  /*/Graph transpose()
  {
    Graph transposed_graph = this;
    for (auto& v : transposed_graph.vertexIncidents)
    {
      v.clear();
    }
    for(int v = 0; v < size(); v++)
    {
      for(auto& to : vertexIncidents[v])
      {
        transposed_graph.addEdge(v, to);
      }
    }
    return transposed_graph;
    //return std::move(transposed_graph);
  }*/

  ~Graph()
  {
    clear();
  }

}; // Graph

#endif /* GRAPH_HPP */
