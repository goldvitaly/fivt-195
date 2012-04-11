#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <memory>


class Graph
{
public: // declaration of types
	class iIncidence;
	typedef std::unique_ptr<iIncidence> incList;
	
public: // declaration of methods
	Graph();
	~Graph(); // нужно ли явно определять стандартные конструктор и деструктор у невиртуального класса?

	void addVertex(incList);
	
	void addEdge(size_t, size_t);
	void delEdge(size_t, size_t);
	
	size_t size() const;
	const iIncidence& operator[] (int) const;
	
private:
	std::vector<incList> incLists;
};

#endif
