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

#include "IncidentType.hpp"

//60

class Graph
{
 public:
  std::vector<IncidentType*> edge;

  Graph()
  {
     
  }

  void AddVertex(IncidentType* incident_type)
  {
    edge.push_back(incident_type);
  }
  void AddEdge(int source, int destination)
  {
    if (edge.size() < std::max(source, destination))
      throw; // XXX: Throw OutOfRangeException
      edge.resize(std::max(source, destination));
    edge[source]->AddEdge(destination);
  }
  void RemoveEdge(int source, int destination)
  {
    if (edge.size() < std::max(source, destination))
      return;
    edge[source]->RemoveEdge(destination);
  }
  bool IsConnected(int source, int destination)
  {
    if (edge.size() < std::max(source, destination))
      return 0;
    return edge[source]->IsConnectedTo(destination);
  }
  
 private:
}; // Graph

#endif /* GRAPH_HPP */
