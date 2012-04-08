#include "graph.hpp"
#include "dfs.hpp"
#include "kosaraju.hpp"

#include <iostream>
#include <cstdlib>
#include <set>
#include <memory>

using namespace std;
using namespace graph;
using namespace algo;

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
		g.connect(rand() % testSize, rand() % testSize);
	KosarajuMaker kosmaker(g);
	auto comps = kosmaker.make();
	cerr << comps.size() << endl;
	return true;
}

int main()
{
	srand(43);
	for(int i = 0; i < 100; ++i)
		if(!testDFS(10000))
			return -1;

	for(int i = 0; i < 100; ++i)
		if(!testStrongComps(500))
			return -1;
	return 0;
}
