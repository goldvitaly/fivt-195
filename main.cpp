#include <cstring>
#include "graph.h"
#include "dijkstra.h"
#include <iostream>

using namespace std;

const int n = 50, m = 100;

vector<int> Dijkstra_(const vector<pair<int, int>> *g, int s)
{ 
	const int INF = 1<<30;
	vector<int> d(n, INF),  p (n);
	d[s] = 0;
	priority_queue < pair<int,int> > q;
	q.push (make_pair (0, s));
	while (!q.empty()) 
	{
		int v = q.top().second,  cur_d = -q.top().first;
		q.pop();
		if (cur_d > d[v])  continue;
 
		for (int j = 0; j < (int)g[v].size(); ++j)
		{
			int to = g[v][j].first,
				len = g[v][j].second;
			if (d[v] + len < d[to])
			{
				d[to] = d[v] + len;
				p[to] = v;
				q.push (make_pair (-d[to], to));
			}
		}
	}
	return d;
}

int main()
{
	srand(time(NULL));
	Graph<int> g;
	vector<pair<int, int>> gr[n];
	bool a[n][n] = {};
	Edge<int> tmp;
	int x;
	for (int i = 1; i <= n; i++)
		if (rand() % 2 == 0)
			g.addVertex(std::unique_ptr<Incidents<int>>(new VecIncidents<int>));
		else
			g.addVertex(std::unique_ptr<Incidents<int>>(new SetIncidents<int>));	

	for (int i = 0; i < m; i++)
	{
		x = rand() % n;
		tmp.to = rand() % n;
		tmp.weight = rand() % n;
		if (a[x][tmp.to])
			continue;
		g.addEdge(x, tmp);
		gr[x].push_back(make_pair(tmp.to, tmp.weight));
		swap(x, tmp.to);
		g.addEdge(x, tmp);
		gr[x].push_back(make_pair(tmp.to, tmp.weight));
		
	}
	vector<int> d(Dijkstra<int>(g).findPath(0));
	vector<int> res(Dijkstra_(gr, 0));
	
	for (int i = 0; i < n; i++)
		if (res[i] != d[i])
			cout<< "WRONG";
	cout<< "\nFinished\n";
	return 0;
}
