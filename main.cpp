#include <cstring>
#include "graph.h"
#include "dijkstra.h"
#include <iostream>

using namespace std;

const int N = 100;

int main()
{
	/*
	srand(time(NULL));
	Graph g;
	int a[N][N];
	vector<int> gr[N];
	for (int i = 0; i < N; i++)
		if (rand() % 2 == 0)
			g.addVertex(std::unique_ptr<Incidents>(new VecIncidents));
		else
			g.addVertex(std::unique_ptr<Incidents>(new SetIncidents));
	int cnt = 0;
	for (int i = 0; i < N; i++)
	{
		Edge tmp;
		int x = rand() % N;
		tmp.to = rand();
		tmp.weight = rand();
		a[x][tmp.to] = tmp.weight;
	}
	for (int i = 0; i < N; i++)
		for (Incidents::Iterator itr = g.getIncidents(i).begin(); itr != g.getIncidents(i).end(); ++itr)
			if (a[i][(*itr).to] != (*itr).weight)
				cout<< "WRONG_GRAPH\n";
			else
				cnt--;
	if (cnt != 0)
		cout<< "WRONG_GRAPH\n";
		* */
	freopen("input.txt", "r", stdin);
	Graph<int> g;
	int n, m;
	cin>> n>> m;
	Edge<int> tmp;
	int x;
	for (int i = 1; i <= n; i++)
		if (i % 2 == 0)
			g.addVertex(std::unique_ptr<Incidents<int>>(new VecIncidents<int>));
		else
			g.addVertex(std::unique_ptr<Incidents<int>>(new SetIncidents<int>));	

	for (int i = 0; i < m; i++)
	{
		cin>>x>> tmp.to>> tmp.weight;
		//cout<< x<<' '<<tmp.to<<'\n';
		g.addEdge(x, tmp);
		swap(x, tmp.to);
		//cout<< x<<' '<<tmp.to<<'\n';
		g.addEdge(x, tmp);
	}
	vector<int> d(Dijkstra<int>(g).findPath(0));
	for (int i = 0; i < n; i++)
		cout<< d[i]<<' ';
	cout<< "\nFinished";
	return 0;
}
