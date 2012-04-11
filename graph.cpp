#include "graph.hpp"

size_t graph::size() const
{
	return vertices.size();
}
void graph::clear()
{
	vertices.clear();
}
void graph::add_vertex(incidence* t)
{
	vertices.push_back(std::unique_ptr<incidence>(t));
}
const incidence& graph::operator[] (size_t v) const
{
	return *vertices[v];
}
void graph::add_edge(size_t from, size_t to)
{
	m_assert(from < size() && to < size(), vertex number out of bound);
	vertices[from]->add_edge(to);
}
void graph::add_unoriented_edge(size_t ver1, size_t ver2)
{
	add_edge(ver1, ver2);
	add_edge(ver2, ver1);
}
void graph::delete_edge(size_t from, size_t to)
{
	m_assert(from < size() && to < size(), vertex number out of bound);
	vertices[from]->delete_edge(to);
}
void graph::delete_unoriented_edge(size_t ver1, size_t ver2)
{
	delete_edge(ver1, ver2);
	delete_edge(ver2, ver1);
}
