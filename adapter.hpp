#pragma once

#include "graph.hpp"

#include <vector>
#include <map>
#include <memory>

namespace graph
{

template<typename VType>
class Adapter
{
public:
	Adapter(Graph& gr, const std::vector<VType>& values);
	unsigned add(std::unique_ptr<Node> node, const VType& name);
	unsigned add(std::unique_ptr<Node> node, const VType& name, const std::vector<VType>& friends);

	template<typename NodeType>
	unsigned add(const VType& name);

	template<typename NodeType>
	unsigned add(const VType& name, const std::vector<VType>& friends);

	void connect(const VType& v1, const VType& v2);
	bool areConnected(const VType& v1, const VType& v2) const;
	const Node& getNode(const VType& v) const;

private:
	Graph& g;
	std::map<VType, unsigned> index;
};

template<typename VType>
Adapter<VType>::Adapter(Graph& gr, const std::vector<VType>& values) : g(gr)
{
	for(size_t v = 0; v < values.size(); ++v)
		index[values[v]] = v;
}

template<typename VType>
unsigned Adapter<VType>::add(std::unique_ptr<Node> node, const VType& name)
{
	unsigned v = g.add(std::move(node));
	index[name] = v;
	return v;
}

template<typename VType>
unsigned Adapter<VType>::add(std::unique_ptr<Node> node, const VType& name, const std::vector<VType>& friends)
{
	unsigned v = g.add(std::move(node));
	index[name] = v;
	for(const VType& u : friends)
		connect(name, u);
	return v;
}

template<typename VType>
template<typename NodeType>
unsigned Adapter<VType>::add(const VType& name)
{
	return add(std::unique_ptr<Node>(new NodeType()), name);
}

template<typename VType>
template<typename NodeType>
unsigned Adapter<VType>::add(const VType& name, const std::vector<VType>& friends)
{
	return add(std::unique_ptr<Node>(new NodeType()), name, friends);
}

template<typename VType>
void Adapter<VType>::connect(const VType& v1, const VType& v2)
{
	return g.connect(index[v1], index[v2]);
}

template<typename VType>
bool Adapter<VType>::areConnected(const VType& v1, const VType& v2) const
{
	return g.areConnected(index.find(v1)->second, index.find(v2)->second);
}

template<typename VType>
const Node& Adapter<VType>::getNode(const VType& v) const
{
	return g.getNode(index.find(v)->second);
}

}