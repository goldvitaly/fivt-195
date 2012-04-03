#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <iostream>
#include "Incidents.h"

using namespace std;

class Graph
{
private:
    vector< unique_ptr<Vertex> > graph;
public:
    Graph() {};
    void addEdge(int from, int to)
    {
        graph[from]->addNeighbour(to);
    }
    void addUndirEdge(int from, int to)
    {
        addEdge(from, to);
        addEdge(to, from);
    }
    void delEdge(int from, int to)
    {
        graph[from]->delNeighbour(to);
    }
    void delUndirEdge(int from, int to)
    {
        delEdge(from, to);
        delEdge(to, from);
    }
    void addVertex(unique_ptr<Vertex> newElem)
    {
        graph.emplace_back(move(newElem));
    }
    size_t numVertices() const
    {
        return graph.size();
    }
    bool isConnect(size_t num1, size_t num2) const
    {
        return graph[num1]->isConnect(num2);
    }
    void resize(size_t n)
    {
        if (n > graph.size())
        {
            for (size_t i = graph.size(); i < n; i++)
            {
                unique_ptr<Vertex> X(new IncidenceList);
                addVertex(move(X));
            }
        }
        else
        {
            for (size_t i = n; i < graph.size(); i++)
                for (size_t j = 0; j < n; j++)
                    if ((*graph[j]).isConnect(i))
                        (*graph[j]).delNeighbour(i);
            for (; n != graph.size();)
                graph.pop_back();
        }
    }
    void foreachIncidence(size_t num, IncidenceCallBack& cb)
    {
        graph[num]->incedents(cb);
    }
};


inline void testGraph()
{
    ifstream fin("testbuilding.in");
    ofstream fout("testbuilding.out");
    Graph G;
    int n, m, x, y;
    fin >> n >> m;
    G.resize(n);
    char t;
    ICBprint testOut(fout);
    for (int i = 0; i < m; i++)
    {
        fin >> t >> x >> y;
        if (t == '+')
            G.addUndirEdge(x, y);
        if (t == '-')
            G.delUndirEdge(x, y);
        if (t == '?')
        {
            if (y == -1)
            {
                G.foreachIncidence(x, testOut);
                fout << endl;
            }
            else
            {
                fout << G.isConnect(x, y) << endl;
                fout << endl;
            }
        }
    }

    fin.close();
    fout.close();
}

#endif
