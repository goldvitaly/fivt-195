#include "graph.hpp"
#include "algo.hpp"
#include <iostream>

using namespace std;
using namespace graph;
using namespace algo;

//Create graph with different nodes representation; test it.
void test1(unsigned vsize)
{
	Graph g;
	for(unsigned i = 0; i < vsize / 2; ++i)
		g.add(new TableNode());
	for(unsigned i = vsize / 2; i < vsize; ++i)
		g.add(new ListNode());
	for(unsigned i = 0; i < vsize; ++i)
		for(unsigned j = i; j < i + 3 && j < vsize; ++j)
			g.connect(i, j);
	for(unsigned v = 0; v < vsize; ++v)
	{
		cout << v << ":" << endl;
		for(auto it : g.getNode(v))
			cout << "\t" << it << endl;
	}
}

//Count components in non-oriented graph. The answer must be equal to "comps" argument.
int test2(int vsize, int comps)
{
	Graph g;
	for(int i = 0; i < vsize; ++i)
		g.add(new ListNode());
	for(int i = 0; i < comps; i++)
		for(int v = i + comps; v < vsize; v += comps)
		{
			if(v - i < 0)
				continue;
			g.connect(v, i);
			g.connect(i, v);
		}
	DFSMaker dfser(g);
	dfser.dfs();
	return dfser.componentsCount();
}

int main()
{
	test1(10);
	int size2 = 1000000;
	if(test2(size2, 33) != 33)
		cerr << "Test 2 failed!" << endl;
	return 0;
}
