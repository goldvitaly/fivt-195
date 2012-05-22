#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include "incidents.h"

class Graph
{
	public:
		void addVertex(std::unique_ptr<Incidents> vInc)
		{
			inc.push_back(std::move(vInc));
		}
		
		void setIncidents(int vertex, std::unique_ptr<Incidents> vInc)
		{
			inc[vertex] = std::move(vInc);
		}
		const Incidents& getIncidents(int vertex) const
		{
			return *inc[vertex];
		}
		
		void addEdge(int from, int to)
		{
			inc[from]->add(to);
		}
		
		void removeEdge(int from, int to)
		{
			inc[from]->remove(to);
		}
		
		void checkEdge(int from, int to) const
		{
			inc[from]->check(to);
		}
		
		int size() const 
		{
			return inc.size();
		}
	private:
		std::vector<std::unique_ptr<Incidents> > inc;
};

#endif /* GRAPH_H */

