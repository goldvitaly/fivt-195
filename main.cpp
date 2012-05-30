#include <cstring>
#include <iostream>
#include <cmath>
#include "graph.h"
#include "dijkstra.h"

using namespace std;

const int n = 100, m = 1000;

void Floyd(vector<vector<int>> &a)
{
	for (int k = 0; k < n; k++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				a[i][j] = min(a[i][j], a[i][k] + a[k][j]);	
}
int main()
{
	const int INF = 1<< 29;
	srand(time(NULL));
	Graph<int> g;
	vector<vector<int>> a(n, vector<int> (n, INF));
	for (int i = 0; i < n; i++)
		a[i][i] = 0;
	Edge<int> tmp;
	int x;
	for (int i = 0; i < n; i++)
		if (rand() % 2 == 0)
			g.addVertex(std::unique_ptr<Incidents<int>>(new VecIncidents<int>));
		else
			g.addVertex(std::unique_ptr<Incidents<int>>(new SetIncidents<int>));	
			
	for (int i = 0; i < m; i++)
	{
		x = rand() % n;
		tmp.to = rand() % n;
		tmp.weight = rand() % n;
		if (a[x][tmp.to] != INF)
			continue;
		g.addEdge(x, tmp);
		a[x][tmp.to] = tmp.weight;
		swap(x, tmp.to);
		g.addEdge(x, tmp);
		a[x][tmp.to] = tmp.weight;
		
	}
	DijkstraInfo<int> d(Dijkstra<int>(g).doDijkstra(0));
	Floyd(a);
	for (int i = 0; i < n; i++)
	{
		if (d.isPath(i))
		{
			if (a[0][i] != d.distTo(i))
			{
				cout<< "WRONG";
			}
		}
		else
		{
			if (a[0][i] != INF)
				cout<< "WRONG";
		}
	}
	cout<< "\nFinished\n";
	return 0;
}
