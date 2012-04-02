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
		assert(g.getNode(v).getFriends().size() == (unsigned)min(3, vsize - v));
}

//Count components in non-oriented graph. The answer must be equal to "comps" argument.
void test2(int vsize, int comps)
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
	assert(solver.getComponentsCount() == comps);
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
	assert(comps[0] == comps[2] && comps[0] == comps[3] && comps[0] == comps[5]);
	assert(comps[1] != comps[0]);
	assert(comps[4] != comps[0]);
	assert(comps[1] != comps[4]);
}

void test4()
{
	Graph g;
	g.add(new ListNode(), {2, 3, 8});
	g.add(new ListNode(), {7});
	g.add(new ListNode(), {6});
	g.add(new ListNode(), {0});
	g.add(new ListNode());
	g.add(new ListNode(), {1, 7, 9});
	g.add(new ListNode(), {0});
	g.add(new ListNode(), {5});
	g.add(new ListNode());
	g.add(new ListNode(), {1});
	GraphSolver solver(g);
	solver.makeTarjanAlgo();
	const vector<int>& comps = solver.getComponents();
	for(unsigned v = 0; v < g.size(); ++v)
		cerr << v << ":" << comps[v] << endl;
	
}

int main()
{
	cerr << "--- Simple tests --- " << endl;
	cerr << "Test 1 start" << endl;
	test1(10);
	cerr << "Test 1 OK" << endl;
	cerr << "Test 2 start" << endl;
	for(int i = 1; i <= 1000000; i *= 10)
		test2(i, min(33, i));
	cerr << "Test 2 OK" << endl;
	cerr << "Test 3 start" << endl;
	test3();
	cerr << "Test 3 OK" << endl;
	cerr << "Test 4 start" << endl;
	test4();
	cerr << "Test 4 OK" << endl;
	return 0;
}
