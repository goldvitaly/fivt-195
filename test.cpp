#include "graph.hpp"
#include "dfs.hpp"
#include "kosaraju.hpp"
#include "tarjan.hpp"
#include "weighted.hpp"

#include <iostream>
#include <cstdlib>
#include <set>
#include <memory>
#include <cmath>

using namespace std;
using namespace graph;
using namespace algo;

void printComps(list<list<unsigned>> comps)
{
	for(auto& comp : comps)
	{
		for(unsigned v : comp)
			cout << v << " ";
		cout << endl;
	}
}

void sortComps(list<list<unsigned>>& comps)
{
	for(auto& comp : comps)
		comp.sort();
	comps.sort();
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
	sortComps(comps1);
	sortComps(comps2);
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

bool primitiveWeightedTest(size_t testSize)
{
	WeightedGraph<int> test;
	for(unsigned i = 0; i < testSize; ++i)
		test.add(unique_ptr<Node>(new ListNode()));
	for(unsigned i = 0; i < testSize - 1; ++i)
	{
		test.connect(i, i + 1, i * i);
		test.connect(i + 1, i, i * i);
	}
	for(unsigned v = 0; v < testSize; ++v)
	{
		for(auto edge : test.getNode(v))
		{
			unsigned u = edge.first;
			int w = edge.second;
			if(abs((int)v - (int)u) > 2 || v == u)
			{
				cerr << "Vertices " << v << " and " << u << " are connected" << endl;
				return false;
			}

			if(w != pow(min(u, v), 2))
			{
				cerr << "Wrong weight between " << v << " and " << u << " : " << w << endl;
				return false;
			}
		}
	}
	cerr << "Primitive weighted test (size " << testSize << ") OK" << endl;
	return true;
}

int main()
{
	srand(43);

	for(int i = 0; i < 100; ++i)
		if(!testDFS(1000))
			return -1;

	for(int i = 0; i < 100; ++i)
		if(!testStrongComps(1000))
			return -1;
	for(size_t i = 1; i < 100; ++i)
		if(!primitiveWeightedTest(i))
			return -1;
	return 0;
}
