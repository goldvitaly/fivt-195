#ifndef TEST_FLOW_H_INCLUDED
#define TEST_FLOW_H_INCLUDED

#include <iostream>
#include <ostream>
#include <algorithm>
#include <vector>
#include <set>
#include <ctime>
#include "Graph.h"
#include "Max_Flow.h"

typedef std::vector< std::vector< std::pair<size_t, size_t> > > TypeTestGraph;

inline void makeGraph(size_t numVertices, size_t numEdges, int maxWeight, Graph<int>* G, TypeTestGraph* testG)
{
    testG->resize(numVertices);
    G->resize(numVertices);
    for (size_t i = 0; i < numEdges; i++)
    {
        size_t from = rand() % numVertices;
        size_t to = rand() % numVertices;
        int weight = rand() % maxWeight;
        G->addEdge(from, to, weight);
        (*testG)[from].push_back(std::make_pair(to, weight));
    }
}

inline bool dfsFF(std::vector< std::vector<int> >* g, std::vector<char>* used, size_t x, size_t to)
{
    if (x == to)
        return true;
    (*used)[x] = true;
    for (size_t v = 0; v < g->size(); v++)
        if ((*g)[x][v] > 0 && !(*used)[v] && dfsFF(g, used, v, to))
        {
            (*g)[x][v]--;
            (*g)[v][x]++;
            return true;
        }
    return false;
};

inline MaxFlowInfo<int> trueAlgoFordFalkerson(const TypeTestGraph& tG, size_t from, size_t to)
{
    MaxFlowInfo<int> res;
    size_t n = tG.size();
    std::vector<std::vector<int> > matr(n);
    for (size_t i = 0; i < n; i++)
        matr[i].assign(n, 0);
    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < tG[i].size(); j++)
        {
            int t = tG[i][j].first;
            int w = tG[i][j].second;
            matr[i][t] += w;
        }
    std::vector<size_t> prev;
    std::vector<char> used(n, false);
    while (dfsFF(&matr, &used, from, to))
    {
        used.assign(n, false);
        res.addFlow(1);
    }
    return res;
}

inline bool runTestFlow(size_t numVertices, size_t numEdges, int maxWeight, std::ofstream& out)
{
    time_t start = clock();
    Graph<int> G;
    TypeTestGraph tG;
    makeGraph(numVertices, numEdges, maxWeight, &G, &tG);
    std::vector<int> trueDist(numVertices);
    std::vector<char> trueIsReached(numVertices);

    MaxFlow<int> flowDijkstra(G);
    size_t from = rand() % numVertices, to = rand() % numVertices;
    while (to == from)
        to = rand() % numVertices;
    MaxFlowInfo<int> pflowInfo = flowDijkstra.calcFlow(from, to);
    out << numVertices << " " << numEdges << " ";
    out << (double (clock()) - start) / CLOCKS_PER_SEC << " ";
   
    MaxFlowInfo<int> trueFlowInfo = trueAlgoFordFalkerson(tG, from, to);
 
    bool res = true;
    res = (pflowInfo.valueFlow() == trueFlowInfo.valueFlow());    
    return res;
}


inline void testFlowStress()
{
    srand(51518905);
    const size_t qTests = 20, maxN = 100, maxM = 2000, maxWeight = 20;
    bool res;
    std::ofstream out;
    out.open("Test_maxflow.log");
    out << "numVertices numEdges time(sec) result" << std::endl;
    for (size_t i = 0; i < qTests; i++)
    {
        srand(rand());
        size_t numVertices = (rand() % maxN) + 2;
        size_t numEdges = (rand() % maxM) + 1;
        size_t curMaxWeight = (rand() % maxWeight) + 1;
        res = runTestFlow(numVertices, numEdges, curMaxWeight, out);
        if (res)
            out << "OK";
        else
        {
            std::cerr << "Incorrect" << std::endl;
            out << "Incorrect";
        }
        out << std::endl;
    }
}

class testFlow
{
private:
    struct Edge
    {
        size_t from, to;
        int w;
        Edge(size_t _from, size_t _to, int _w): from(_from), to(_to), w(_w) {}
    };
    struct Test
    {
        std::vector<Edge> edges;
        int ans;
        size_t numVertices, st, fin;
    };
    std::vector<Test> tests;
    void makeTests()
    {
        {
            Test cur;
            cur.edges.push_back(Edge(0, 1, 2));
            cur.edges.push_back(Edge(1, 2, 1));
            cur.ans = 1;
            cur.st = 0;
            cur.fin = 2;
            cur.numVertices = 3;
            tests.push_back(cur);
        }
        
        {
            Test cur;
            cur.edges.push_back(Edge(0, 1, 2));
            cur.edges.push_back(Edge(1, 2, 1));
            cur.edges.push_back(Edge(0, 2, 3));
            cur.ans = 4;
            cur.st = 0;
            cur.fin = 2;
            cur.numVertices = 3;
            tests.push_back(cur);
        }
        {
            Test cur;
            cur.edges.push_back(Edge(0, 1, 2));
            cur.edges.push_back(Edge(1, 2, 1));
            cur.edges.push_back(Edge(0, 2, 3));
            cur.edges.push_back(Edge(2, 3, 6));
            cur.ans = 4;
            cur.st = 0;
            cur.fin = 3;
            cur.numVertices = 5;
            tests.push_back(cur);
        }
        {
            Test cur;
            cur.edges.push_back(Edge(0, 1, 2));
            cur.edges.push_back(Edge(1, 2, 1));
            cur.edges.push_back(Edge(0, 2, 3));
            cur.edges.push_back(Edge(2, 3, 6));
            cur.edges.push_back(Edge(3, 0, 6));
            cur.ans = 4;
            cur.st = 0;
            cur.fin = 3;
            cur.numVertices = 5;
            tests.push_back(cur);
        }
    };

public:
    void startTestingEasy()
    {
        makeTests();
        for (size_t i = 0; i < tests.size(); i++)
        {
            Graph<int> G;
            Test curTest = tests[i];
            G.resize(curTest.numVertices);

            for (size_t j = 0; j < tests[i].edges.size(); j++)
            {
                G.addEdge(curTest.edges[j].from, curTest.edges[j].to, curTest.edges[j].w);
            }
            MaxFlow<int> flowDijkstra(G);
            int from = curTest.st, to = curTest.fin;
            MaxFlowInfo<int> pflowInfo = flowDijkstra.calcFlow(from, to);
            if (pflowInfo.valueFlow() == curTest.ans)
                std::cout << i + 1 << " OK" << std::endl;
            else
                std::cout << i + 1 << " WA" << std::endl;
        }
    }
};
#endif
