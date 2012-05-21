#include "graph.hpp"
#include "dfs.hpp"
#include "kosaraju.hpp"
#include "tarjan.hpp"
#include "weighted.hpp"
#include "dijkstra.hpp"
#include "adapter.hpp"

#include <iostream>
#include <cstdlib>
#include <memory>
#include <cmath>
#include <cassert>
#include <algorithm>

using namespace std;
using namespace graph;
using namespace algo;

const size_t tests_size = 200;
const size_t tests_count = 3000;

void printComps(list<list<unsigned>>& comps)
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

bool primitiveAdapterTest()
{
	Graph gr;
	Adapter<string> g(gr, {});
	/*
	g.add(std::unique_ptr<Node>(new TableNode()), "A");
	g.add(std::unique_ptr<Node>(new TableNode()), "B");
	g.add(std::unique_ptr<Node>(new TableNode()), "C", {"B"});
	g.add(std::unique_ptr<Node>(new TableNode()), "D");
	g.add(std::unique_ptr<Node>(new TableNode()), "E", {"A", "B"});
	g.add(std::unique_ptr<Node>(new TableNode()), "F", {"A", "B", "C", "D", "E"});
	*/
	g.add<TableNode>("A");
	g.add<TableNode>("B");
	g.add<TableNode>("C", {"B"});
	g.add<TableNode>("D");
	g.add<TableNode>("E", {"A", "B"});
	g.add<TableNode>("F", {"A", "B", "C", "D", "E"});
	assert(g.areConnected("C", "B"));
	assert(g.areConnected("F", "D"));
	assert(g.areConnected("E", "A"));
	assert(!g.areConnected("A", "B"));
	assert(!g.areConnected("C", "E"));
	assert(!g.areConnected("D", "F"));
	assert(g.getNode("F").getFriends().size() == 5);
	cerr << "Primitive adapter test OK" << endl;
	return true;
}

bool primitiveDijkstraTest()
{
	WeightedGraph<double> g;
	for(unsigned v = 0; v < 4; ++v)
		g.add<TableNode>();
	double wg[4];
	for(size_t i = 0; i < 4; ++i)
		wg[i] = rand() % 100 / 3.0;
	g.connect(0, 1, wg[0]);
	g.connect(0, 2, wg[1]);
	g.connect(1, 3, wg[2]);
	g.connect(2, 3, wg[3]);
	DijkstraMaker<double, SumPath<double>> maker = DijkstraMaker<double, SumPath<double>>(g);
	auto& res = maker.make(0);
	double dist[4];
	for(size_t i = 0; i < 4; ++i)
		dist[i] = res[i].length();
	assert(dist[0] == 0);
	assert(dist[1] == wg[0]);
	assert(dist[2] == wg[1]);
	assert(dist[3] == min(wg[0] + wg[2], wg[1] + wg[3]));
	cerr << "Primitive Dijkstra test OK" << endl;
	return true;
}

bool monoDijkstraTest(size_t testSize)
{
	WeightedGraph<double> g;
	for(unsigned v = 0; v < testSize; ++v)
		g.add<TableNode>();
	int b = rand() % 1000 + 1;
	int a = b + rand() % b;
	size_t edges = testSize * a / b;
	for(size_t e = 0; e < edges; ++e)
	{
		unsigned from = rand() % g.size();
		unsigned to = rand() % g.size();
		g.connect(from, to, 1.0);
	}
	DijkstraMaker<double, SumPath<double>> maker(g);
	auto& res = maker.make(0);
	vector<double> iter;
	iter.assign(g.size(), HUGE_VAL);
	iter[0] = 0;
	queue<unsigned> q;
	q.push(0);
	vector<double> proc;
	proc.assign(g.size(), false);
	proc[0] = true;
	while(!q.empty())
	{
		unsigned v = q.front();
		q.pop();
		for(const auto& p : g.getNode(v))
		{
			if(!proc[p.first])
			{
				proc[p.first] = true;
				iter[p.first] = iter[v] + 1;
				q.push(p.first);
			}
		}
	}
	for(unsigned v = 0; v < g.size(); ++v)
	{
		assert(iter[v] == res[v].length());
	}
	cerr << "Mono Dijkstra test (E = " << a << "/" << b << " V) OK" << endl;
	return true;
}

bool dijkstraTraceTest(size_t testSize)
{
	WeightedGraph<double> g;
	for(unsigned v = 0; v < testSize; ++v)
		g.add<ListNode>();
	size_t edges = testSize * testSize / 16;
	for(unsigned e = 0; e < edges; ++e)
	{
		unsigned from = rand() % testSize;
		unsigned to = rand() % testSize;
		double w = rand() % 10000 / 7.0;
		g.connect(from, to, w);
	}

	DijkstraMaker<double, TraceSumPath<double>> maker(g);
	DijkstraMaker<double, SumPath<double>> simple(g);
	const auto& res = maker.make(0);
	const auto& check = simple.make(0);
	double sum = 0;
	for(unsigned v = 0; v < g.size(); ++v)
	{
		const auto& path = res.at(v).path();
		double length = res.at(v).length();
		double length2 = path.empty() ? HUGE_VAL : 0;
		auto it = path.begin();
		unsigned prev = *it;
		while(++it != path.end())
		{
			auto wg =  g.getWeight(prev, *it);
			length2 += *min_element(wg.begin(), wg.end());
			prev = *it;
		}
		double checkLength = check.at(v).length();
		assert(length == length2);
		assert(length == checkLength);
		if(length != HUGE_VAL)
			sum += length;
	}
	cerr << "Dijkstra trace test OK (paths of " << sum << " checked)" << endl;
	return true;
}

int main()
{
	srand(43);

	for(size_t i = 0; i < tests_count; ++i)
		if(!testDFS(tests_size))
			return -1;

	for(size_t i = 0; i < tests_count; ++i)
		if(!testStrongComps(tests_size))
			return -1;

	for(size_t i = tests_size; i < tests_size + tests_count; ++i)
		if(!primitiveWeightedTest(i))
			return -1;

	if(!primitiveAdapterTest())
		return -1;

	for(size_t i = 0; i < tests_count; ++i)
		if(!primitiveDijkstraTest())
			return -1;

	for(size_t i = 0; i < tests_count; ++i)
		if(!monoDijkstraTest(tests_size))
			return -1;

	for(size_t i = 0; i < tests_count; ++i)
		if(!dijkstraTraceTest(tests_size))
			return -1;

	cerr << "All tests OK" << endl;

	return 0;
}
