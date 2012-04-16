#ifndef GENERATORS_HPP
#define GENERATORS_HPP

#include "../../Graph/Graph.hpp"

#include <cstdlib>
#include <algorithm>

namespace generators
{
class IGenerator // я пока не определился с общим форматом генераторов, поэтому класс не несет никакой полезной информации и нужен только для структуры
{
public:
	virtual ~IGenerator () {}
};

class Random: public IGenerator
{
public:
	virtual ~Random() {}
	graph::Graph gen(size_t numVertices, size_t numEdges,  std::unique_ptr<graph::IIncidence> (insGetter)())
	{
		graph::Graph graph;
		for (size_t i = 0; i < numVertices; i++)
			graph.addVertex(insGetter());
		for (size_t i = 0; i < numEdges; i++)
			graph.addEdge(rand()%graph.size(), rand()%graph.size());
		return std::move(graph);
	}
};
	
} // namespace generators

#endif
