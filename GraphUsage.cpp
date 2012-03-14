#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cassert>

#include "Graph.hpp"

using namespace std;

void DFS(int vertex, Graph& graph)
{
  printf("%d:\n", vertex);
  for(IncidentType::Iterator it = graph.vertex[0]->begin(); it != graph.vertex[0]->end(); ++it)
  {
    printf("%d ", *it);
  }
}

int main()
{
  Graph G;
  G.addVertex(new BitmaskIncident);
  G.addVertex(new BitmaskIncident);
  G.addVertex(new BitmaskIncident);
  G.addVertex(new BitmaskIncident);
  G.addVertex(new BitmaskIncident);
  G.addVertex(new BitmaskIncident);
  G.addVertex(new BitmaskIncident);
  G.addVertex(new BitmaskIncident);
  G.addVertex(new BitmaskIncident);
  G.addEdge(0, 1);
  G.addEdge(0, 4);
  G.addEdge(0, 5);
  G.addEdge(0, 6);
  G.addEdge(0, 4);
  G.addEdge(0, 8);

  DFS(0, G);

  return 0;
}
