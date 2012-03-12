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

//60

class Graph
{
 public:
  std::vector<IncidentType*> vertex;

  Graph()
  {
     
  }

  void addVertex(IncidentType* incident_type)
  {
    vertex.push_back(incident_type);
  }
  void addEdge(int source, int destination)
  {
    if (vertex.size() < std::max(source, destination))
      throw new std::exception;
    vertex.resize(std::max(source, destination));
    vertex[source]->AddEdge(destination);
  }
  void removeEdge(int source, int destination)
  {
    if (vertex.size() < std::max(source, destination))
      return;
    vertex[source]->RemoveEdge(destination);
  }
  bool isConnected(int source, int destination)
  {
    if (vertex.size() < std::max(source, destination))
      return 0;
    return vertex[source]->IsConnectedTo(destination);
  }
  
 private:
}; // Graph

#endif /* GRAPH_HPP */
