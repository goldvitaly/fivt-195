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

#include "IncidentType.hpp"

class Graph
{
 public:
  std::vector<IncidentType*> vertex;

  Graph()
  {
     
  }

  void addVertex(IncidentType* incident_type)
  {
    printf("Adding vertex %d\n", vertex.size());
    vertex.push_back(incident_type);
  }
  void addEdge(int source, int destination)
  {
    printf("Adding edge %d %d\n", source, destination);
    if (vertex.size() < std::max(source, destination))
      throw new std::exception;
    vertex[source]->addEdge(destination);
  }
  void removeEdge(int source, int destination)
  {
    if (vertex.size() < std::max(source, destination))
      throw new std::exception;
    vertex[source]->removeEdge(destination);
  }
  bool isConnected(int source, int destination) const
  {
    if (vertex.size() < std::max(source, destination))
      throw new std::exception;
    return vertex[source]->isConnectedTo(destination);
  }

  ~Graph()
  {
  }
  
 private:
}; // Graph

#endif /* GRAPH_HPP */
