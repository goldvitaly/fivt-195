#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <memory>
#include <utility>
#include "incident.hpp"

class Graph
{
public:
	Graph (){}
	template <typename IncidentClass> void AddVertex (size_t Vertex)
	{
		while (Vertex >= _Inc.size()) _Inc.push_back(new IncidentClass());
	}
	void RemoveVertex (size_t Vertex)
	{
		for (size_t i = 0; i < Vertex; ++i)
			for (size_t j = Vertex; j < _Inc.size(); ++j)
				_Inc[i]->RemoveIncident(j);

		while (Vertex < _Inc.size()) _Inc.pop_back();
	}
	void AddIncident (size_t VertexA, size_t VertexB)
	{
		_Inc[VertexA]->AddIncident(VertexB);
	}
	bool CheckIncident (size_t VertexA, size_t VertexB) const
	{
		if (VertexA >= _Inc.size()) return false;
		return _Inc[VertexA]->CheckIncident(VertexB);
	}
	size_t VertexNum () const
	{
		return _Inc.size();
	}
	size_t IncidentNum () const
	{
		size_t num = 0;
		for (size_t i = 0; i < _Inc.size(); ++i)
			num += _Inc[i]->IncidentNum();
		return num;
	}
	const Incident* GetIncident (size_t Vertex) const
	{
		return _Inc[Vertex];
	}
private:
	std::vector <Incident*> _Inc;
};

#endif /* GRAPH_HPP */