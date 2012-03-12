#include "Graph.hpp"
#include "Incidents.hpp"
#include "SetIncidents.hpp"
#include "VectorIncidents.hpp"
#include <cstdlib>
#include <iostream>

using namespace std;
int main(int argc, char** argv) {
	Graph graph;
	for(int i = 0; i < 5; ++i){
		graph.addVertex(new SetIncidents());
	}
	graph.addEdge(1, 4);
	graph.addEdge(2, 4);
	graph.addEdge(0, 3);
	graph.addEdge(2, 1);
	graph.addEdge(2, 3);

	for(auto vertex_id: graph.getIncidents(2)){
		cout << vertex_id << ' ';
	}
	return 0;
}

