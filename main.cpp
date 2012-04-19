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
				g.addEdge(i, j, rand()%100);
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
int main() {
	testTarjan();
	stressTestDijkstra(1000);
	return 0;
}
