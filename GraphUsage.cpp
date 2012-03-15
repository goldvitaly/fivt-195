#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <stack>

#include "Graph.hpp"

using namespace std;


class GraphAlgorithms
{
 public:
  static vector<int> getStrongComponentDivision(const Graph& G)
  {
    vertexCount = G.vertexCount();
    tin.assign(vertexCount, 0);
    tup.assign(vertexCount, 0);
    isInStack.assign(vertexCount, 0);
    component.resize(vertexCount);
    curTime = 1;
    componentCount = 0;
    for(int i = 0; i < vertexCount; i++)
    {
      if (tin[i] == 0)
        dfs(i, G);
    }
    return component;
  }

 private:
  static vector<int> tin;
  static vector<int> tup;
  static vector<int> component; 
  static vector<bool> isInStack;
  static stack<int> S;
  static int vertexCount;
  static int curTime;
  static int componentCount;

  static void dfs(int vertex, const Graph& graph)
  {
    S.push(vertex);
    isInStack[vertex] = 1;
    tin[vertex] = tup[vertex] = curTime++;
    for(IncidentType::Iterator it = graph.vertex[vertex]->begin(); it != graph.vertex[vertex]->end(); ++it)
    {
      int to = *it;
      if (tin[to] == 0)
      {
        dfs(to, graph);
        tup[vertex] = min(tup[vertex], tup[to]);
      }
      else
        if (isInStack[to])
          tup[vertex] = min(tup[vertex], tin[to]);
    }
    if (tin[vertex] == tup[vertex])
    {
      component[vertex] = componentCount;
      isInStack[vertex] = 0;
      while (!S.empty() && S.top() != vertex)
      {
        component[S.top()] = componentCount;
        isInStack[S.top()] = 0;
        S.pop();
      } 
      S.pop();
      componentCount++;
    }
  }
};

vector<int> GraphAlgorithms::tin;
vector<int> GraphAlgorithms::tup;
vector<int> GraphAlgorithms::component;
vector<bool> GraphAlgorithms::isInStack;
stack<int> GraphAlgorithms::S;
int GraphAlgorithms::vertexCount;
int GraphAlgorithms::curTime;
int GraphAlgorithms::componentCount;

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
  G.addEdge(1, 4);
  G.addEdge(4, 0);
  G.addEdge(0, 4);
  G.addEdge(0, 5);
  G.addEdge(0, 6);
  G.addEdge(0, 4);
  G.addEdge(8, 4);
  G.addEdge(0, 8);

  vector<int> component = GraphAlgorithms::getStrongComponentDivision(G);
  for(int i = 0; i < component.size(); i++)
  {
    printf("vertex %d lies in %d\n", i, component[i]);
  }

  return 0;
}
