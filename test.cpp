#include "graph.hpp"
#include "algo.cpp"
#include <iostream>

using namespace std;
using namespace graph;
using namespace algo;

//Create graph with different nodes representation; test it.
void test1(unsigned vsize)
{
	Graph g;
	Node* nodes[vsize];
	for(unsigned i = 0; i < vsize / 2; ++i)
		g.add(nodes[i] = new TableNode());
	for(unsigned i = vsize / 2; i < vsize; ++i)
		g.add(nodes[i] = new ListNode());
	for(unsigned i = 0; i < vsize; ++i)
		for(unsigned j = i; j < i + 3 && j < vsize; ++j)
			g.connect(i, j);
	for(unsigned v = 0; v < vsize; ++v)
	{
		cout << v << ":" << endl;
		IteratorWrapper it = g.begin(v);
		while(it != g.end(v))
		{
			cout << "\t" << *it << endl;
			++it;
		}
	}
	for(unsigned i = 0; i < vsize; ++i)
		delete nodes[i];
}

//Count components in non-oriented graph. The answer must be equal to "comps" argument.
void test2(int vsize, int comps)
{
	Graph g;
	TableNode nodes[vsize];
	for(int i = 0; i < vsize; ++i)
		g.add(nodes + i);
	for(int i = 0; i < comps; i++)
		for(int v = i + comps; v < vsize; v += comps)
		{
			if(v - i < 0)
				continue;
			g.connect(v, i);
			g.connect(i, v);
		}
	cout << algo::countComponents(g) << endl;
}

int main()
{
	test1(10);
	for(int i = 1; i <= 15; ++i)
		test2(15, i);
	return 0;
}
