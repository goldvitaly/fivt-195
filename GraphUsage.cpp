/*
 * =====================================================================================
 *
 *       Filename:  Testing.cpp
 *
 *    Description:  Testing for GraphAlgorithms and Graph classes
 *
 *        Version:  1.0
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <vector>
#include <typeinfo>

#include "Graph.hpp"
#include "GraphAlgorithms.hpp"

using namespace std;

int main()
{
  Graph G = graph_algorithms::getRandomGraph(10, 0.1, 1);

  graph_algorithms::StronglyConnectedComponentsFinder finder(G);
  vector<int> component = finder.getStronglyConnectedComponents();
  G.setVertexIncidenceType(4, new SetIncidence);
  for(int i = 0; i < component.size(); i++)
  {
    printf("vertex %d lies in %d\n", i, component[i]);
  }

  for(auto& v : G.vertexIncidents)
    cout << v.getType() << endl;

  cout << G.size() << endl;
  G.clear();

  return 0;
}
