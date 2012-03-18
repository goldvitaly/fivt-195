#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <vector>

#include "Graph.hpp"
#include "GraphAlgorithms.hpp"

using namespace std;

int main()
{
  Graph G = GraphAlgorithms::getRandomGraph(100, 0.01, 1);

  vector<int> component = GraphAlgorithms::getStronglyConnectedComponentsDivision(G);
  for(int i = 0; i < component.size(); i++)
  {
    printf("vertex %d lies in %d\n", i, component[i]);
  }

  cout << G.size() << endl;
  G.clear();

  return 0;
}
