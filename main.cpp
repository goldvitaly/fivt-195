#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include "Incidents.h"

using namespace std;

class Graph
{
private:
	vector< unique_ptr<pVertex> > graph;
public:
	void addEdge(const int from, const int to)
	{
		(*graph[from]).addNeighbour(to);
	};
	void addUnDirEdge(const int from, const int to)
	{
		addEdge(from, to);
		addEdge(to, from);
	};
	void delEdge(const int from, const int to)
	{
		(*graph[from]).delNeighbour(to);
	};
	void delUnDirEdge(const int from, const int to)
	{
		delEdge(from, to);
		delEdge(to, from);
	};
	void addVertex(const unique_ptr<pVertex> &newElem)
	{
		graph.emplace_back(newElem);
	};
	const size_t numVertex()
	{
		return graph.size();
	};
	void resize(const int n)
	{
		if (n > graph.size())
		{	
			for (int i = graph.size(); i < n; i++)
			{
				pVertex* Y = new Vvectorbool;
				unique_ptr<pVertex> X(Y);
				addVertex(X);
			}
		}
		else
		{
			for (int i = n; i < graph.size(); i++)
				for (int j = 0; j < n; j++)
					if ((*graph[j]).isConnect(i))
						(*graph[j]).delNeighbour(i);
			for (; n != graph.size();)
				graph.pop_back();
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