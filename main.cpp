#include <cstring>
#include "graph.h"
#include <iostream>
using namespace std;
int main()
{
	srand(time(NULL));
	Graph g;
	int N = 100;
	bool a[N][N];
	for (int i = 0; i < N; i++)
		if (rand() % 2 == 0)
			g.addVertex(std::unique_ptr<Incidents>(new VecIncidents));
		else
			g.addVertex(std::unique_ptr<Incidents>(new SetIncidents));
	int cnt = 0;
	for (int i = 0; i < N; i++)
	{
		int x = rand() % N, y = rand() % N;
		if (!a[x][y])
			g.addEdge(x, y), cnt++;
		a[x][y] = true;
	}
	for (int i = 0; i < N; i++)
		for (Incidents::Iterator itr = g.getIncidents(i).begin(); itr != g.getIncidents(i).end(); ++itr)
			if (!a[i][*itr])
				cout<< "WRONG";
			else
				cnt--;
	if (cnt != 0)
		cout<< "WRONG";
	return 0;
}
