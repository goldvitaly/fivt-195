#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include "Incidents.h"

using namespace std;


class pGraph
{
private:
	vector<pVertex *> graph;
public:
	void addEdge(const int from, const int to)
	{
		graph[from]->addNeighbour(to);
	}
	void addDualEdge(const int from, const int to)
	{
		addEdge(from, to);
		addEdge(to, from);
	}
	void delEdge(const int from, const int to)
	{
		graph[from]->delNeighbour(to);
	}
	void delDualEdge(const int from, const int to)
	{
		delEdge(from, to);
		delEdge(to, from);
	}
	void addVertex(const int v, const string &type = "int")
	{
		if (v >= graph.size())
			graph.resize(v + 1);
		if (graph[v] == NULL)
		{	
			if (type == "bool")
			{
				graph[v] = new Vvectorbool;
			}
			else
			if (type == "int")
			{
				graph[v] = new Vvectorint;
			}	
			else
			if (type == "set")
			{
				graph[v] = new Vset;
			}			
			else
			if (type == "map")
			{
				graph[v] = new Vmap;
			}
			else
			if (type == "list")
			{
				graph[v] = new Vlist;
			}
		}
	}
	void delVertex(const int v)
	{
		graph[v]->clear();
	}
	const size_t numVertex()
	{
		return graph.size();
	}
	void resize(const int n)
	{
		graph.resize(n);
		for (int i = graph.size(); i < n; i++)
			addVertex(i);
	}
};


class Graph
{
private:
	vector<Vertex> graph;
public:
	void addEdge(const int from, const int to)
	{
		graph[from].addNeighbour(to);
	};
	void addDualEdge(const int from, const int to)
	{
		addEdge(from, to);
		addEdge(to, from);
	};
	void delEdge(const int from, const int to)
	{
		graph[from].delNeighbour(to);
	};
	void delDualEdge(const int from, const int to)
	{
		delEdge(from, to);
		delEdge(to, from);
	};
	void addVertex(const string &type = "int")
	{
		graph.push_back(type);
	};
	const size_t numVertex()
	{
		return graph.size();
	};
	void resize(const int n)
	{
		if (n > graph.size())
		{	
			graph.resize(n);
		}
		else
		{
			for (int i = n; i < graph.size(); i++)
				for (int j = 0; j < n; j++)
					if (graph[j].isConnect(i))
						graph[j].delNeighbour(i);
			resize(n);
		}
	};
};



void test()
{
	ifstream fin("test.in");
	ofstream fout("test.out");
	Graph G;
	int n, m, x, y;
	fin >> n >> m;
	G.resize(n);
	for (int i = 0; i < m; i++)
	{
		fin >> x >> y;
		G.addEdge(x, y);
	}
	fin.close();
	fout.close();
}

int main()
{
	test();
	return 0;
}