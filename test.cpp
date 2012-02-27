#include "graph.hpp"
#include <iostream>

using namespace std;

int main()
{
	//basic-vertex test
	Graph<Vertex<string>, int> g;
	string n[] = {"MSC", "NY", "LA", "London"};
	Vertex<string> v[4];
	for(int i = 0; i < 4; i++)
	{
		v[i] = Vertex<string>(n[i]);
		g.addVertex(v[i]);
	}
	g.link(n[0], n[3], 100);
	g.link(n[3], n[1], 300);
	g.link(n[1], n[2], 50);
	g.link(n[2], n[0], 400);
	for(int i = 0; i < 4; i++)
	{
		Graph<Vertex<string>, int>::links_list_t list = g.getLinksList(n[i]);
		Graph<Vertex<string>, int>::links_iterator_t iter = list.begin();
		while(iter != list.end())
			cout << iter++->first << endl;
		cout << "---" << endl;
	}
	return 0;
}
