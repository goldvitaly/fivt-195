#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include "Incidents.h"
#include <memory>
#include <utility>

using namespace std;
using std::unique_ptr;

class Graph
{
private:
	vector< unique_ptr<pVertex> > graph;
public:
	void addEdge(const int from, const int to)
	{
		graph[from]->addNeighbour(to);
	}
	void addUnDirEdge(const int from, const int to)
	{
		addEdge(from, to);
		addEdge(to, from);
	}
	void delEdge(const int from, const int to)
	{
		graph[from]->delNeighbour(to);
	}
	void delUnDirEdge(const int from, const int to)
	{
		delEdge(from, to);
		delEdge(to, from);
	}
	void addVertex(unique_ptr<pVertex> newElem)
	{
		graph.emplace_back(move(newElem));
	}
	size_t numVertex() const
	{
		return graph.size();
	}
	bool isConnect(size_t num1, size_t num2) const
	{
		return graph[num1]->isConnect(num2);
	}
	void resize(const size_t n)
	{
		if (n > graph.size())
		{
			for (size_t i = graph.size(); i < n; i++)
			{
				unique_ptr<pVertex> X(new IncidenceList);
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
	void callVertex(size_t num, ICB& cb)
	{
		graph[num]->incedents(cb);
	}
};



void test()
{
	ifstream fin("test.in");
	ofstream fout("test.out");
	Graph G;
	int n, m, x, y;
	fin >> n >> m;
	G.resize(n);
	char t;
	ICBprint And(fout);
	for (int i = 0; i < m; i++)
	{
		fin >> t >> x >> y;
		if (t == '+')
			G.addUnDirEdge(x, y);
		if (t == '-')
			G.delUnDirEdge(x, y);
		if (t == '?')
		{
			if (y == -1)
			{
				G.callVertex(x, And);
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

int main()
{
	test();
	return 0;
}
