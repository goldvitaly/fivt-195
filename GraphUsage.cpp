#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "Graph.hpp"

using namespace std;

void DFS(int vertex, Graph* graph)
{
}

int main()
{
  Graph G;
  G.addVertex(new VectorIncident);
  G.addVertex(new VectorIncident);
  G.addEdge(0, 1);
  IncidentType::Iterator it = G.vertex[0]->begin();
  while (it != G.vertex[0]->end())
  {
    printf("Preparing for printing: ");
    cout << *it << endl;
    ++it;
  }
  //G.AddVertex(new ListIncident);
  //G.AddEdge(1, 2);
  //G.AddEdge(2, 0);
  return 0;
}
