#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include "incidents.h"

template<class Weight>
class Graph
{
	public:
		void addVertex(std::unique_ptr<Incidents<Weight>> vInc)
		{
			inc.push_back(std::move(vInc));
		}
		
		void setIncidents(int vertex, std::unique_ptr<Incidents<Weight>> vInc)
		{
			inc[vertex] = std::move(vInc);
		}
		const Incidents<Weight>& getIncidents(int vertex) const
		{
			return *inc[vertex];
		}
		
		void addEdge(int from, Edge<Weight> to)
		{
			inc[from]->add(to);
		}
		
		void removeEdge(int from, Edge<Weight> to)
		{
			inc[from]->remove(to);
		}
		
		void checkEdge(int from, Edge<Weight> to) const
		{
			inc[from]->check(to);
		}
		
		int size() const 
		{
			return inc.size();
		}
	private:
		std::vector<std::unique_ptr<Incidents<Weight>>> inc;
};

#endif /* GRAPH_H */

