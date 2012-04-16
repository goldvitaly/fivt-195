#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <memory>

#include "IIncidence.hpp"

namespace graph
{
class Graph
{
public: // declaration of types
	typedef std::unique_ptr<IIncidence> incList;
	
public: // declaration of methods
	Graph();
	Graph(const Graph&) = delete;
	Graph(Graph&&) = default;
	~Graph(); // нужно ли явно определять стандартные конструктор и деструктор у невиртуального класса?

	void addVertex(incList);
	void clear();
	
	void addEdge(size_t, size_t);
	void delEdge(size_t, size_t);
	
	size_t size() const;
	const IIncidence& operator[] (int) const;
	
private:
	std::vector<incList> incLists;
};
} // namespace graph

#endif
