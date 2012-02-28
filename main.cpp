#include <cstdlib>
#include <iostream>

#include "Graph.hpp"
#include "Incidents.hpp"
#include "VectorIncidents.hpp"

using namespace std;
int main(int argc, char** argv) {
	Graph graph;
	for(int i=0;i<5;++i){
		graph.addVertex(new VectorIncidents());
	}
	graph.addEdge(1,4);
	graph.addEdge(2,4);
	graph.addEdge(0,3);
	graph.addEdge(2,1);
	graph.addEdge(2,3);
	for(auto vertex_id: *graph.getIncidents(2)){
	//for(auto vertex_id = graph.getIncidents(2)->begin(),end=graph.getIncidents(2)->end();vertex_id!=end; ++vertex_id){
		(cout<<vertex_id<<' ').flush();
	}
	return 0;
}

