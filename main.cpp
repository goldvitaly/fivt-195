#include "AllShortestPaths.hpp"
#include "Graph.hpp"
#include "Incidents.hpp"
#include "MatrixIncidents.hpp"
#include "SetIncidents.hpp"
#include "ShortestPaths.hpp"
#include "StronglyConnectedComponents.hpp"
#include "StronglyConnectedComponentsInfo.hpp"
#include "TrivialStronglyConnectedComponents.hpp"
#include "VectorIncidents.hpp"
#include "MaxFlow.hpp"
#include <cstdlib>
#include <iostream>
#include <set>
#include <map>
#include <memory>
using namespace std;

typedef std::unique_ptr<Incidents<NoWeight> > PIncidents;
typedef std::vector<size_t> Coloring;

Graph<NoWeight> genGraph(int mask){
	Graph<NoWeight> graph;
	graph.addVertex(PIncidents(new VectorIncidents<NoWeight>()));
	graph.addVertex(PIncidents(new MatrixIncidents(4)));
	graph.addVertex(PIncidents(new VectorIncidents<NoWeight>()));
	graph.addVertex(PIncidents(new SetIncidents<NoWeight>()));
	for(int i=0; i < 4; ++i){
		for(int j = 0; j < 4; ++j){
			int curEdge = 4 * i + j;
			if(mask & (1 << curEdge)){
				graph.addEdge(i, j);
			}
		}
	}
	return graph;
}

Graph<int> genRandomWeightedGraph(size_t size){
	Graph<int> g;
	for(size_t i=0; i<size; ++i){
		Incidents<int>* ptr;
		if(rand()&1){
			ptr = new VectorIncidents<int>();
		}
		else
			ptr = new SetIncidents<int>();
		g.addVertex(std::unique_ptr< Incidents<int> >(ptr));
	}
	for(size_t i=0; i<size; ++i){
		for(size_t j=0; j<size; ++j){
			if(rand()&1){
				g.addEdge(i, j, rand()%1000);
			}
		}
	}
	return g;
}
bool checkColoringsEqual(const Coloring& a, const Coloring& b){
	assert(a.size()==b.size());
	for(size_t i = 0; i < a.size(); ++i){
		for(size_t j=0; j < b.size(); ++j){
			if((a[i]==a[j]) != (b[i]==b[j]))
				return false;
		}
	}
	return true;
}
bool testGraph(const Graph<NoWeight>& graph){
	Coloring received = StronglyConnectedComponents(graph).getComponents().ids();
	Coloring expected = TrivialStronglyConnectedComponents(graph).getColoring();
	return checkColoringsEqual(received, expected);
}

void testTarjan(){
	for(int i = 0; i < (1 << 16); ++i){
		if(!testGraph(genGraph(i))){
			cerr<<"Test failed on 4-vertex graph";
			exit (1);
		}
	}
}

void testDijkstraPathes(int n){
	for(int i=0;i<n;++i){
		Graph<int> g(genRandomWeightedGraph(1000));
		int start = rand() % g.size();
		ShortestPaths<int,int> sp(g);
		ShortestPathsInfo<int, int> spi = sp.calculate(start);
		for(size_t to = 0; to < g.size(); ++to){
			if(!spi.length(to)){
			}
			else{
				int len = *spi.length(to);
				const std::vector<Incidents<int>::Iterator> path = spi.path(to);
				int gettedLen = 0;
				for(const Incidents<int>::Iterator& inc: path){
					gettedLen += (*inc).weight;
				}
				assert(len == gettedLen);
			}
		}
	}
}

void stressTestDijkstra(int n){
	typedef std::vector< std::vector< boost::optional<int> > > Result;
	for(int i=0;i<n;++i){
		Graph<int> g(genRandomWeightedGraph(10));
		Result received(g.size());
		Result expected = AllShortestPaths<int>(g).calculate();
		for(size_t i=0; i<g.size(); ++i){
			received[i] = ShortestPaths<int>(g).calculate(i).allLengths();
		}
		assert(expected == received);
	}
}

	
void testFlow(int times){
	for(int z=0;z<times;++z){
		Graph<int> g;
		int n = rand()%100;
		if(n<2){
			--z;
			continue;
		}
		for(int i=0;i<n;++i){
			g.addVertex(std::unique_ptr<Incidents<int> >(new VectorIncidents<int>()));
		}
		vector<size_t> a;
		size_t start = rand() % g.size();
		for(int i=0;i<n;++i){
			if((size_t)i!=start)
				a.push_back(i);
		}
		size_t end = start;
		while(end == start)
			end = rand() % g.size();
		int flow = 0;
		for(int i=0;i<20;++i){
			int w = rand() % 1000;
			flow += w;
			random_shuffle(a.begin(), a.end());
			size_t prev = start;
			for(int t=0;prev!=end;++t){
				if(start == a[t])
					continue;
				g.addEdge(prev, a[t], w);
				prev = a[t];
			}
		}
		int foundFlow = MaxFlow<int>(g).flow(start, end);
		assert(foundFlow == flow);
	}
}

void handMadeTestFlow(){
	Graph<int> g;
	g.addVertex(std::unique_ptr<Incidents<int> >(new VectorIncidents<int>()));
	g.addVertex(std::unique_ptr<Incidents<int> >(new VectorIncidents<int>()));
	assert(MaxFlow<int>(g).flow(0,1) == 0);
	g.addEdge(0,1, 42);
	assert(MaxFlow<int>(g).flow(0,1) == 42);
	assert(MaxFlow<int>(g).flow(1,0) == 0);
	g.addVertex(std::unique_ptr<Incidents<int> >(new VectorIncidents<int>()));
	g.addEdge(2,0, 423);
	assert(MaxFlow<int>(g).flow(2, 1) == 42);
}
int main() {
	//testTarjan();
	//stressTestDijkstra(1000);
	//testDijkstraPathes(100);
	handMadeTestFlow();
	testFlow(100);
	return 0;
}