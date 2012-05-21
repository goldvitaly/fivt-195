#include "graph.hpp"
#include "graph_algorithm.hpp"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <unistd.h>

int main ()
{    
    Graph<Edge> G;
    G.AddVertex<VIntIncident<Edge>>();
    G.AddVertex<VIntIncident<Edge>>();
    G.AddVertex<VIntIncident<Edge>>();
    G.AddVertex<VIntIncident<Edge>>();
    G.AddIncident(0, 1, Edge(1));
    G.AddIncident(1, 2, Edge(1));
    G.AddIncident(2, 3, Edge(1));
    G.AddIncident(0, 2, Edge(1));
    G.AddIncident(1, 3, Edge(1));
    MaxFlowAlgorithm<Edge, Path<Edge>, PathComp<Path<Edge>>, Relax<Edge, Path<Edge>>, int> mfa(G);
    
    std::cout << mfa(0, 2);
    
    return 0;
}
