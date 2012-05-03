#ifndef TEST_DFS_TARJAN_H_INCLUDED	
#define TEST_DFS_TARJAN_H_INCLUDED

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <ctime>
#include "Graph.h"
#include "Dfs_tarjan.h"

typedef std::vector< std::vector<size_t> > TypeTestGraph;

inline void genGraph(size_t numVertices, size_t numEdges, Graph &G, TypeTestGraph &testG)
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

//Finding the list of vertices, which are reachable from vertice V
inline void getReachList(const TypeTestGraph &tG, size_t v, std::set<size_t> &reachList, std::vector<char> &used)
{
	used[v] = true;
	reachList.insert(v);
	for (size_t i = 0; i < tG[v].size(); i++)
	{
		size_t to = tG[v][i];
		if (!used[to])
		{
			getReachList(tG, to, reachList, used);
		}
	}
}

inline bool canReach(size_t from, size_t to, const std::vector< std::set<size_t> > &reachList)
{
	return (reachList[from].find(to) != reachList[from].end());
}

//For each vertex V we are searching for a set of vertices U that 
//V~>U and U~>V (A~>B mean that vertex with number B is reachable from vertex A)
//Then check this set with strongly connected component for this vertex V
inline bool checkAnsTarjan(const Graph &G, const TypeTestGraph &testG, std::vector<TypeComponent> &components)
{
   std::vector< std::set<size_t> > reachList(G.numVertices());
	for (size_t v = 0; v < G.numVertices(); v++)
   {
      std::vector<char> used(G.numVertices(), false);
      getReachList(testG, v, reachList[v], used);  
   }
	size_t qVertices = 0;
   for (size_t numComp = 0; numComp < components.size(); numComp++)
		qVertices += components[numComp].size();
	if (qVertices != G.numVertices())
		return false;	
	for (size_t numComp = 0; numComp < components.size(); numComp++)
	{
		std::sort(components[numComp].begin(), components[numComp].end());
		for (size_t i = 0; i < components[numComp].size(); i++)
		{
			size_t curVertex = components[numComp][i];
			std::vector<size_t> trueComponent;
         for (size_t v = 0; v < G.numVertices(); v++)
            if (canReach(curVertex, v, reachList) && canReach(v, curVertex, reachList)) //check curVertex~>v and v~>curVertex
               trueComponent.push_back(v);
         if (trueComponent != components[numComp])
			{
				return false;
			}
		}
	}	
	return true;
};

inline bool genTestTarjan(size_t numVertices, size_t numEdges)
{
	time_t start = clock();
	Graph G;
	TypeTestGraph tG;
	std::vector<TypeComponent> components;
	genGraph(numVertices, numEdges, G, tG);
	findStroglyConnectedComponents(G, components);
   std::cout << numVertices << " " << numEdges << " ";
   std::cout << (double (clock()) - start) / CLOCKS_PER_SEC << " ";
	bool res = checkAnsTarjan(G, tG, components);
	return res;
}


inline void testTarjan()
{
   srand(515115);
	const size_t qTests = 20, maxN = 500, maxM = 2000;
   bool res;
	std::cout << "numVertices numEdges time(sec) result" << std::endl;
   for (size_t i = 0; i < qTests; i++)
	{
		srand(rand());
		size_t numVertices = (rand() % maxN) + 1;
		size_t numEdges = (rand() % maxM) + 1;
		res = genTestTarjan(numVertices, numEdges);
		if (res)
			std::cout << "OK";
		else
			std::cout << "Incorrect";
		std::cout << std::endl;
	}
}

#endif
