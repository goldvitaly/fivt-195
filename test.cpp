#include "graph.hpp"
#include "algo.hpp"
#include <iostream>

using namespace std;
using namespace graph;
using namespace algo;

//Create graph with different nodes representation; test it.
void test1(int vsize)
{
	Graph g;
	for(int i = 0; i < vsize / 2; ++i)
		g.add(new TableNode());
	for(int i = vsize / 2; i < vsize; ++i)
		g.add(new ListNode());
	for(int i = 0; i < vsize; ++i)
		for(int j = i; j < i + 3 && j < vsize; ++j)
			g.connect(i, j);
	for(int v = 0; v < vsize; ++v)
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
	return dfser.getComponentsCount();
}

void test3()
{
	Graph g;
	for(int i = 0; i < 6; ++i)\
		g.add(new ListNode());
	g.connect(0, 3); g.connect(3, 2); g.connect(2, 0);
	g.connect(3, 5); g.connect(5, 3);
	g.connect(2, 4); g.connect(5, 4);
	DFSMaker dfser(g);
	dfser.makeKosarajuAlgo();
	const vector<int>& comps = dfser.getComponents();
	for(size_t i = 0; i < comps.size(); ++i)
		cout << i << ":" << comps[i] << endl;
}

int main()
{
	test1(10);
	int size2 = 1000000;
	if(test2(size2, 33) != 33)
		cerr << "Test 2 failed!" << endl;
	test3();
	return 0;
}
