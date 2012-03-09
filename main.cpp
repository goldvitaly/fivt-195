#include <iostream>
#include <typeinfo>
#include "graph.hpp"
#include "vector_incidence.hpp"
#include "set_incidence.hpp"

using namespace std;

int main()
{
	graph g;
	
	g.add_vertex(new vector_incidence());
	g.add_vertex(new vector_incidence());
	g.add_vertex(new vector_incidence());
	g.add_vertex(new set_incidence());
	g.add_edge(0, 1);
	g.add_edge(0, 2);
	g.add_edge(0, 3);
	g.add_edge(3, 1);
	
	for (int i = 0; i < (int)g.size(); i++)
	{
		cout << i << ": ";
		for (auto it: g[i])
			cout << it << " ";
		cout << endl;
	}
	
	return 0;
}
