#include "graph.hpp"
#include <iostream>

using namespace std;

int main()
{
	cout << "Sizeof Vertex<int, int>: " << sizeof(Vertex<int, int> ) << endl;
	//int-vertex test
	const int int_sz = 10000;
	Graph<Vertex<int, int>, int> g;
	for(int i = 0; i < int_sz; i++)
	{
		int id = i;
		int val = i * i % 1000001 + i % 100001;
		Vertex<int, int> v(id, val);
		g.addVertex(v);
	}
	for(int i = 0; i < int_sz; i++)
	{
		int v1 = i;
		int v2 = i * i % int_sz;
		int edge = i * i * i % 10001;
		g.link(v1, v2,  edge);
	}
	return 0;
}
