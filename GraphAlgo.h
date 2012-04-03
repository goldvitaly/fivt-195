#ifndef GRAPHALGO_H_INCLUDED
#define GRAPHALGO_H_INCLUDED

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
typedef vector< size_t > TypeComponent;


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
inline void getReachList(const TypeTestGraph &tG, size_t v, set<size_t> &reachList, vector<char> &used)
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



inline bool canReach(size_t from, size_t to, const vector< set<size_t> > &reachList)
{
    return (reachList[from].find(to) != reachList[from].end());
}

//For each vertex V we are searching for a set of vertices U that
//V~>U and U~>V (A~>B mean that vertex with number B is reachable from vertex A)
//Then check this set with strongly connected component for this vertex V
inline bool checkAnsTarjan(const Graph &G, const TypeTestGraph &testG, vector<TypeComponent> &components)
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
            size_t curVertex = components[numComp][i];
            vector<size_t> trueComponent;
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


class infoDFSTarjan
{
public:

    vector<char> used;
    vector<char> isRoot;
    size_t timer;
    vector<size_t> lowLink;
    stack<size_t> stackVertices;

    infoDFSTarjan (size_t numVertices): used(numVertices, false), isRoot(numVertices, false), timer(0), lowLink(numVertices) {}
};

class DFSTarjan : public IncidenceCallBack
{
private:
    Graph* G;
    infoDFSTarjan* info;
    size_t par;
    vector<TypeComponent>* components;

    void updLowLink(size_t v, size_t par, vector<size_t> &lowLink, vector<char> &isRoot)
    {
        if (par < (*G).numVertices()) // it should be in DFS for parent after recursive from its child v
        {
            if (lowLink[v] < lowLink[par])
            {
                lowLink[par] = lowLink[v];
                isRoot[par] = false;
            }
        }
    }
public:
    DFSTarjan (Graph &_G, size_t _par, infoDFSTarjan &_info, vector<TypeComponent> &_components)
    {
        G = &_G;
        par = _par;
        info = &_info;
        components = &_components;
    }
    ~DFSTarjan() {}
    void operator()(size_t v)
    {
        if (info->used[v])
        {
            updLowLink(v, par, info->lowLink, info->isRoot);
            return;
        }
        info->lowLink[v] = info->timer++;
        info->used[v] = true;
        info->stackVertices.push(v);
        info->isRoot[v] = true;
        DFSTarjan newDFS(*this);
        newDFS.par = v;
        (*G).foreachIncidence(v, newDFS);

        if (info->isRoot[v])
        {
            vector<size_t> component;
            while (!info->stackVertices.empty())
            {
                size_t k = info->stackVertices.top();
                info->stackVertices.pop();
                component.push_back(k);
                info->lowLink[k] = numeric_limits<size_t>::max();
                if (k == v)
                    break;
            }
            (*components).push_back(component);
        }

        updLowLink(v, par, info->lowLink, info->isRoot);
    }
};

inline void findStroglyConnectedComponents(Graph &G, vector<TypeComponent> &components)
{
    infoDFSTarjan info(G.numVertices());
    DFSTarjan dfs(G, G.numVertices(), info, components);
    for (size_t v = 0; v < G.numVertices();  v++)
        if (!info.used[v])
            dfs(v);
}

inline bool genTestTarjan(size_t numVertices, size_t numEdges)
{
    time_t start = clock();
    Graph G;
    TypeTestGraph tG;
    vector<TypeComponent> components;
    genGraph(numVertices, numEdges, G, tG);
    findStroglyConnectedComponents(G, components);
    cout << numVertices << " " << numEdges << " ";
    cout << (double (clock()) - start) / CLOCKS_PER_SEC << " ";
    bool res = checkAnsTarjan(G, tG, components);
    return res;
}

inline void testTarjan()
{
    srand(515115);
    bool res = genTestTarjan(20, 100);
    cout << res << endl;
    res = genTestTarjan(50, 300);
    cout << res << endl;
    res = genTestTarjan(500, 3000);
    cout << res << endl;
    res = genTestTarjan(1000, 5000);
    cout << res << endl;
}

#endif
