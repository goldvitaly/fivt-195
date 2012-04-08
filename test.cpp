#include "graph.hpp"
#include "dfs.hpp"
#include "kosaraju.hpp"
#include "tarjan.hpp"

#include <iostream>
#include <cstdlib>
#include <set>
#include <memory>

using namespace std;
using namespace graph;
using namespace algo;

void printComps(set<set<unsigned>> comps)
{
	for(auto& comp : comps)
	{
		for(unsigned v : comp)
			cout << v << " ";
		cout << endl;
	}
	cout << "---" << endl;
}

bool testDFS(size_t testSize)
{
	Graph g;
	size_t size = 0;
	size_t root = sqrt(testSize);
	size_t compCount1 = 0;
	while(size < testSize)
	{
		++compCount1;
		size_t compSize = rand() % (root - 1) + 1;
		for(size_t v = 0; v < compSize; ++v)
		{
			vector<unsigned> friends = {(unsigned)(size + (v + 1) % compSize)};
			for(unsigned i = 0; i < 5; ++i)
				friends.push_back(size + rand() % compSize);
			g.add(unique_ptr<Node>(new ListNode()), friends);
		}
		size += compSize;
	}
	DFSMaker dfser(g);
	unsigned compCount2 = 0;
	for(unsigned v = 0; v < size; ++v)
	{
		if(dfser.dfs(v) == COLOR_WHITE)
			++compCount2;
	}
	if(compCount1 != compCount2)
		cerr << "DFS test failed: " << "wrong answer " << compCount2 <<
										 "( correct: " << compCount1 << ")" << endl;
	else
		cerr << "DFS test OK: " << "answer " << compCount2 << endl;
	return compCount1 == compCount2;
}

bool testStrongComps(size_t testSize)
{
	Graph g;
	for(unsigned v = 0; v < testSize; ++v)
		g.add(unique_ptr<Node>(new ListNode()));
	size_t edges = testSize * 3;
	for(unsigned e = 0; e < edges; ++e)
	{
		unsigned u = rand() % testSize;
		unsigned v = rand() % testSize;
		g.connect(u, v);
	}
	KosarajuMaker kosmaker(g);
	auto comps1 = kosmaker.make();
	TarjanMaker tarjmaker(g);
	auto comps2 = tarjmaker.make();
	if(comps1 != comps2)
	{
		cerr << "Strong components test fail: Kosaraju = " << comps1.size() <<
										     ", Tarjan = " << comps2.size() << endl;
		/*printComps(comps1);
		printComps(comps2);*/
	}
	else
		cerr << "Strong components test OK: components = " << comps1.size() << endl;
	return comps1 == comps2;
}

int main()
{
	srand(43);
	for(int i = 0; i < 100; ++i)
		if(!testDFS(10000))
			return -1;

	for(int i = 0; i < 100; ++i)
		if(!testStrongComps(10000))
			return -1;
	return 0;
}
