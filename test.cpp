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
	GraphSolver solver(g);
	solver.dfs();
	return solver.getComponentsCount();
}

//Test Kosaraju. V = 6.
void test3()
{
	Graph g;
	g.add(new ListNode(), {3});
	g.add(new ListNode());
	g.add(new ListNode(), {0, 4});
	g.add(new ListNode(), {2, 5});
	g.add(new ListNode());
	g.add(new ListNode(), {3, 4});
	GraphSolver solver(g);
	solver.makeKosarajuAlgo();
	const vector<int>& comps = solver.getComponents();
	for(size_t i = 0; i < comps.size(); ++i)
		cout << i << ":" << comps[i] << endl;
}

int main()
{
	cout << "Test 1 start" << endl;
	test1(10);
	cout << "Test 1 end" << endl;
	int size2 = 1000000;
	cerr << "Test 2 success" << endl;
	if(test2(size2, 33) != 33)
		cerr << "Oh no, test 2 failed!" << endl;
	cerr << "Test 2 end" << endl;
	cerr << "Test 3 start" << endl;
	test3();
	cerr << "Test 3 end" << endl;
	return 0;
}
