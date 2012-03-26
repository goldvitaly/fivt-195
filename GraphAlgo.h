#include <iostream>
#include "Graph.h"
#include <vector>
#include <algorithm>
#include <stack>
#include <limits>
#include <set>
#include <ctime>

using namespace std;

typedef vector< vector<size_t> > TypeTestGraph;
typedef vector< size_t > typeComponent;

inline void genGraph(const size_t numVertices, const size_t numEdges, Graph &G, TypeTestGraph &testG)
{
	testG.resize(numVertices);
	G.resize(numVertices);
	for (size_t i = 0; i < numEdges; i++)
	{
		size_t from = rand() % numVertices;
		size_t to = rand() % numVertices;
		G.addEdge(from, to);
		testG[from].push_back(to);
	}
}


inline void getReachList(const TypeTestGraph &tG, const size_t v, set<size_t> &reachList, vector<char> &used)
{
	used[v] = true;
	reachList.insert(v);
	for (size_t i = 0; i < tG[v].size(); i++)
	{
		size_t to = tG[v][i];
		if (used[to] == false)
		{
			getReachList(tG, to, reachList, used);
		}
	}
}

inline bool checkAnsTarjan(const Graph &G, const TypeTestGraph &testG, vector<typeComponent> &components)
{
   vector< set<size_t> > reachList(G.numVertices());
	for (size_t v = 0; v < G.numVertices(); v++)
   {
      vector<char> used(G.numVertices(), false);
      getReachList(testG, v, reachList[v], used);  
   }
   for (size_t numComp = 0; numComp < components.size(); numComp++)
	{
		sort(components[numComp].begin(), components[numComp].end());
		for (size_t i = 0; i < components[numComp].size(); i++)
		{
			int curVertex = components[numComp][i];
			vector<size_t> trueComponent;
         for (size_t v = 0; v < G.numVertices(); v++)
            if (reachList[v].find(curVertex) != reachList[v].end() && reachList[curVertex].find(v) != reachList[curVertex].end())
               trueComponent.push_back(v);
         //cerr << "lol\n";
         if (trueComponent.size() != components[numComp].size() || !equal(trueComponent.begin(), trueComponent.end(), components[numComp].begin()))
				return false;
		}
	}	
	return true;
}

class DFSTarjan : public ICB 
{
private:
	Graph* G;
	size_t par;
	vector<char>* used;
   vector<char>* isRoot;
	size_t* timer;
	vector<size_t>* lowLink;
	stack<size_t>* stackVertices;
	vector<typeComponent>* components;

   void updLowLink(size_t v, vector<size_t>* lowLink, vector<char>* isRoot)
   {
      if (par < (*G).numVertices()) // it should be in DFS for parent after recursive from its child v
      {
         if ((*lowLink)[v] < (*lowLink)[par])
		   {
			   (*lowLink)[par] = (*lowLink)[v];
			   (*isRoot)[par] = false;
		   }
      }
   }
public:
	DFSTarjan(const DFSTarjan &cpy)
	{
		*this = cpy; //copying vector<unique_ptr> is wrong?
	}
	DFSTarjan (Graph &_G, size_t _par, vector<char> &_used, vector<char> &_isRoot, size_t &_timer, vector<size_t> &_lowLink, stack<size_t> &_stack, vector<typeComponent> &_components) 
	{G = &_G; par = _par; used = &_used; isRoot = &_isRoot; timer = &_timer; lowLink = &_lowLink; stackVertices = &_stack; components = &_components;};
	~DFSTarjan() {};
	void operator() (size_t v) 
	{
		if ((*used)[v] == true)
		{
         updLowLink(v, lowLink, isRoot);
         return;
      }
      (*lowLink)[v] = (*timer)++;
		(*used)[v] = true;
		(*stackVertices).push(v);
		(*isRoot)[v] = true;
		DFSTarjan newDFS(*this);
      newDFS.par = v;
		(*G).callVertex(v, newDFS);

		if ((*isRoot)[v])
		{
			vector<size_t> component;
			while (true)
			{
				int k = (*stackVertices).top();
				(*stackVertices).pop();
				component.push_back(k);
				(*lowLink)[k] = numeric_limits<size_t>::max();
				if (k == v)
					break;
			}
			(*components).push_back(component);
		}

      updLowLink(v, lowLink, isRoot);
	}
};

inline void findStroglyConnectedComponents(Graph &G, vector<typeComponent> &components)
{
   time_t start = clock();
	vector<char> used(G.numVertices(), false);
   vector<char> isRoot(G.numVertices(), false);
	vector<size_t> lowLink(G.numVertices());
	size_t timer = 0;
	stack<size_t> st;
	DFSTarjan dfs(G, G.numVertices(), used, isRoot, timer, lowLink, st, components);
	for (size_t v = 0; v < G.numVertices();  v++)
		if (used[v] == false)
			dfs(v);
   cout << (double (clock()) - start) / CLOCKS_PER_SEC << " ";
}

inline bool genTestTarjan(const size_t numVertices, const size_t numEdges)
{
	Graph G;
	TypeTestGraph tG;
	vector<typeComponent> components;
	genGraph(numVertices, numEdges, G, tG);
	findStroglyConnectedComponents(G, components);
   cout << numVertices << " " << numEdges << " ";
   bool res = checkAnsTarjan(G, tG, components);
	return res;
}

inline void testTarjan()
{
   srand(911);
   bool res = genTestTarjan(20, 100);
   cout << res << endl;
   res = genTestTarjan(50, 300);
   cout << res << endl;
   res = genTestTarjan(500, 3000);
   cout << res << endl;
   res = genTestTarjan(1000, 5000);
   cout << res << endl;
}