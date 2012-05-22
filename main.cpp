#include <cstring>
#include "graph.h"
#include "find_strongly_conn_com.h"
#include <iostream>

using namespace std;

const int N = 100;
	

void dfs1(const vector<int> *gr, vector<int> &list, vector<int> &color,  int now)
{
	color[now] = 1;
	for (int i: gr[now])
	{
		if (color[i] == -1)
			dfs1(gr, list, color, i);
	}
	list.push_back(now);
}
void dfs2(const vector<int> *gr_t, vector<int> &color, int number, int now)
{
	color[now] = number;
	for (int i: gr_t[now])
		if (color[i] == -1)
			dfs2(gr_t, color , number, i);
} 


bool checkColors(const vector<int> &color, const vector<int> &id)
{
	//for (int i = 0; i < N; i++)
	//	cout<< id[i]<<' '<< color[i]<<'\n';
	for (int i = 0; i < N; i++)
			for (int j = i + 1; j < N; j++)
				if ((color[i] == color[j] && id[i] != id[j]) ||
					(color[i] != color[j] && id[i] == id[j]))
						return false;			
	return true;		
}

bool testStronglyConnComp(vector<int> *gr, const vector<int> &id)
{
	vector<int> gr_t[N];
	for (int i = 0; i < N; i++)
		for (int j: gr[i])
			gr_t[j].push_back(i);
	
	vector<int> list, color(N, -1);
	for (int i = 0; i < N; i++)
		if(color[i] == -1)
			dfs1(gr, list, color, i);
	
	int number = -1;
	color.assign(N, -1);

	for (int i = list.size() - 1; i >= 0; i--)
		if (color[list[i]] == -1)
		{
			number++;
			dfs2(gr_t, color, number, list[i]);			
		}
	return (checkColors(color, id));
}

int main()
{
	srand(time(NULL));
	Graph g;
	bool a[N][N];
	vector<int> gr[N];
	for (int i = 0; i < N; i++)
		if (rand() % 2 == 0)
			g.addVertex(std::unique_ptr<Incidents>(new VecIncidents));
		else
			g.addVertex(std::unique_ptr<Incidents>(new SetIncidents));
	/*
	g.addEdge(1, 2);
	gr[1].push_back(2);
	
	g.addEdge(2, 3);
	gr[2].push_back(3);
	
	g.addEdge(3, 1);
	gr[3].push_back(1);
	
	g.addEdge(1, 6);
	gr[1].push_back(6);
	
	g.addEdge(6, 3);
	gr[6].push_back(3);
	
	g.addEdge(4, 5);
	gr[4].push_back(5);
	
	
	
	*/
	int cnt = 0;
	for (int i = 0; i < N; i++)
	{
		int x = rand() % N, y = rand() % N;
		if (!a[x][y])
		{
			g.addEdge(x, y), cnt++;
			gr[x].push_back(y);
		}
		a[x][y] = true;
	}
	for (int i = 0; i < N; i++)
		for (Incidents::Iterator itr = g.getIncidents(i).begin(); itr != g.getIncidents(i).end(); ++itr)
			if (!a[i][*itr])
				cout<< "WRONG_GRAPH\n";
			else
				cnt--;
	if (cnt != 0)
		cout<< "WRONG_GRAPH\n";
	
	vector<int> id(FindStronglyConnCom (g).components());
	if (!testStronglyConnComp(gr, id))
		cout<< "WRONG_TARJAN\n";
	
	cout<< "\nFinished";
	return 0;
}
