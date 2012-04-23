#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <iostream>
#include "Incidents.h"

class Graph
{
private:
	std::vector< std::unique_ptr<Vertex> > graph;
public:
   Graph() {};
	void addEdge(int from, int to)
	{
		graph[from]->addNeighbour(to);
	}
	void addUndirEdge(int from, int to)
	{
		addEdge(from, to);
		addEdge(to, from);
	}
	void delEdge(int from, int to)
	{
		graph[from]->delNeighbour(to);
	}
	void delUndirEdge(int from, int to)
	{
		delEdge(from, to);
		delEdge(to, from);
	}
	void addVertex(std::unique_ptr<Vertex> newElem)
	{
		graph.emplace_back(std::move(newElem));
	}
	size_t numVertices() const
	{
		return graph.size();
	}
	bool isConnect(size_t num1, size_t num2) const
	{
		return graph[num1]->isConnect(num2);
	}
	void resize(size_t n)
	{
		if (n > graph.size())
		{
			for (size_t i = graph.size(); i < n; i++)
			{
				std::unique_ptr<Vertex> newVertex(new IncidenceList);
				addVertex(std::move(newVertex));
			}
		}
		else
		{
			for (size_t i = n; i < graph.size(); i++)
				for (size_t j = 0; j < n; j++)
					if ((*graph[j]).isConnect(i))
						(*graph[j]).delNeighbour(i);
			for (; n != graph.size();)
				graph.pop_back();
		}
	}
	void foreachIncidence(size_t num, IncidenceCallBack& cb) const
	{
		graph[num]->incedents(cb);
	}
};

#endif
