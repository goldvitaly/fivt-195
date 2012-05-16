#ifndef TEST_DIJKSTRA_H_INCLUDED
#define TEST_DIJKSTRA_H_INCLUDED

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <ctime>
#include "Graph.h"
#include "Dijkstra.h"

typedef std::vector< std::vector< std::pair<size_t, size_t> > > TypeTestGraph;

inline void makeGraph(size_t numVertices, size_t numEdges, int maxWeight, Graph<int> &G, TypeTestGraph &testG)
{
    testG.resize(numVertices);
    G.resize(numVertices);
    for (size_t i = 0; i < numEdges; i++)
    {
        size_t from = rand() % numVertices;
        size_t to = rand() % numVertices;
        int weight = rand() % maxWeight;
        G.addEdge(from, to, weight);
        testG[from].push_back(std::make_pair(to, weight));
    }
}

inline void trueAlgoDijkstra(TypeTestGraph& tG, size_t from, std::vector<int>* outDist, std::vector<char>* outIsReached)
{
    size_t n = tG.size();
    std::vector<char> mark(n, false);
    std::vector<int> d(n, std::numeric_limits<int>::max());
    d[from] = 0;
    for (size_t t = 0; t < n; t++)
    {
        size_t u = std::numeric_limits<size_t>::max();
        int mn = std::numeric_limits<int>::max();
        for (size_t i = 0; i < n; i++)
            if (!mark[i] && d[i] < mn)
            {
                u = i;
                mn = d[i];
            }
        if (u == std::numeric_limits<size_t>::max())
            break;
        mark[u] = true;
        for (size_t i = 0; i < tG[u].size(); i++)
        {
            size_t to = tG[u][i].first;
            int len = tG[u][i].second;
            if (d[to] > d[u] + len)
                d[to] = d[u] + len;
        }
    }
    *outDist = d;
    *outIsReached = mark;
}

inline void trueAlgoFloyd(TypeTestGraph& tG, size_t from, std::vector<int>* outDist, std::vector<char>* outIsReached)
{
    size_t n = tG.size();
    std::vector< std::vector<char> > isReached(n);
    for (size_t i = 0; i < n; i++)
		isReached[i].assign(n, false);
    std::vector< std::vector<int> > d(n);
    for (size_t i = 0; i < n; i++)
	{
		d[i].assign(n, std::numeric_limits<int>::max());
		d[i][i] = 0;
		isReached[i][i] = true;
	}
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < tG[i].size(); j++)
		{
			int to = tG[i][j].first, weight = tG[i][j].second;
			d[i][to] = std::min(d[i][to], weight);
			isReached[i][to] = true;
		}
	for (size_t k = 0; k < n; k++)
    {
        for (size_t i = 0; i < n; i++)
			for (size_t j = 0; j < n; j++)
				if (isReached[i][k] && isReached[k][j])
				{
					d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);
					isReached[i][j] = true;
				}
	}
	for (size_t i = 0; i < n; i++)
	{
		(*outDist)[i] = d[from][i];
		(*outIsReached)[i] = isReached[from][i];
	}
}
inline bool runTestDijkstra(size_t numVertices, size_t numEdges, int maxWeight)
{
    time_t start = clock();
    Graph<int> G;
    TypeTestGraph tG;
    makeGraph(numVertices, numEdges, maxWeight, G, tG);
    std::vector<int> trueDist(numVertices);
    std::vector<char> trueIsReached(numVertices);

    Dijkstra<int, int> dijkstra(G);
    size_t from = rand() % numVertices;
    ShortestPathInfo<int> pathInfo =  dijkstra.calcDist(from);
	std::cout << numVertices << " " << numEdges << " ";
    std::cout << (double (clock()) - start) / CLOCKS_PER_SEC << " ";
   
    //trueAlgoDijkstra(tG, from, &trueDist, &trueIsReached);
    trueAlgoFloyd(tG, from, &trueDist, &trueIsReached);
 
	bool res = true;
	for (size_t i = 0; i < numVertices; ++i)
        if (!trueIsReached[i])
        {
            if (pathInfo.isReach(i))
            {
                res = false;
                break;
            }
        }
        else
        {
            if (pathInfo.getLength(i) != trueDist[i])
            {
                res = false;
                break;
            }
        }
    return res;
}


inline void testDijkstra()
{
    srand(51518905);
    const size_t qTests = 20, maxN = 100, maxM = 1000, maxWeight = 10000;
    bool res;
    std::cout << "numVertices numEdges time(sec) result" << std::endl;
    for (size_t i = 0; i < qTests; i++)
    {
        srand(rand());
        size_t numVertices = (rand() % maxN) + 1;
        size_t numEdges = (rand() % maxM) + 1;
        size_t curMaxWeight = (rand() % maxWeight) + 1;
        res = runTestDijkstra(numVertices, numEdges, curMaxWeight);
        if (res)
            std::cout << "OK";
        else
            std::cout << "Incorrect";
        std::cout << std::endl;
    }
}

#endif
