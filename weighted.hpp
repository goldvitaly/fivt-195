#pragma once

#include "graph.hpp"

#include <vector>
#include <list>
#include <memory>

namespace graph
{

template<typename WeightType>
class WeightedIterator
{
	typedef typename std::vector<WeightType>::const_iterator w_iter;
	
public:
	WeightedIterator(std::unique_ptr<NodeIterator>& nodeIter, w_iter weightIter);
	WeightedIterator<WeightType>& operator++();
	WeightedIterator<WeightType>& operator--();
	std::pair<unsigned, WeightType> operator*() const;
	bool operator==(const WeightedIterator& wi) const;
	bool operator!=(const WeightedIterator& wi) const;	

private:
	std::unique_ptr<NodeIterator> nodeIterator;
	w_iter weightIterator;

};

template<typename WeightType>
class WeightedNode
{
public:
	WeightedNode(const Node& nd, const std::vector<WeightType>& wg);
	WeightedIterator<WeightType> begin();
	WeightedIterator<WeightType> end();
	std::list<WeightType> getWeight(unsigned v) const;

private:
	const Node& node;
	const std::vector<WeightType>& weights;
};

template<typename WeightType>
class WeightedGraph
{
public:
	WeightedGraph();
	unsigned add(std::unique_ptr<Node> node);
	unsigned add(std::unique_ptr<Node> node, const std::vector<unsigned>& friends, const std::vector<WeightType>& edgesWeights);

	template<typename NodeType>
	unsigned add();

	template<typename NodeType>
	unsigned add(const std::vector<unsigned>& friends, const std::vector<WeightType>& edgesWeights);

	void connect(unsigned v1, unsigned v2, WeightType weight);
	bool areConnected(unsigned v1, unsigned v2) const;
	std::list<WeightType> getWeight(unsigned v1, unsigned v2) const;
	WeightedNode<WeightType> getNode(unsigned v) const;
	size_t size() const;
	const Graph& getBaseGraph();

private:
	Graph graph;
	std::vector<std::vector<WeightType>> weights;
};

template<typename WeightType>
WeightedIterator<WeightType>::WeightedIterator(std::unique_ptr<NodeIterator>& nodeIter, w_iter weightIter) : weightIterator(weightIter)
{
	nodeIterator = std::move(nodeIter);
}

template<typename WeightType>
WeightedIterator<WeightType>& WeightedIterator<WeightType>::operator++()
{
	++*nodeIterator;
	++weightIterator;
	return *this;
}

template<typename WeightType>
WeightedIterator<WeightType>& WeightedIterator<WeightType>::operator--()
{
	--*nodeIterator;
	--weightIterator;
	return *this;
}

template<typename WeightType>
std::pair<unsigned, WeightType> WeightedIterator<WeightType>::operator*() const
{
	return make_pair(**nodeIterator, *weightIterator);
}

template<typename WeightType>
bool WeightedIterator<WeightType>::operator==(const WeightedIterator& wi) const
{
	return *nodeIterator == *wi.nodeIterator;
}

template<typename WeightType>
bool WeightedIterator<WeightType>::operator!=(const WeightedIterator& wi) const
{
	return *nodeIterator != *wi.nodeIterator;
}	

template<typename WeightType>
WeightedNode<WeightType>::WeightedNode(const Node& nd, const std::vector<WeightType>& wg) : node(nd), weights(wg) {}

template<typename WeightType>
WeightedIterator<WeightType> WeightedNode<WeightType>::begin()
{
	return WeightedIterator<WeightType>(node.begin().getBaseIterator(), weights.begin());
}

template<typename WeightType>
WeightedIterator<WeightType> WeightedNode<WeightType>::end()
{
	return WeightedIterator<WeightType>(node.end().getBaseIterator(), weights.end());
}	

template<typename WeightType>
std::list<WeightType> WeightedNode<WeightType>::getWeight(unsigned v) const
{
	std::list<WeightType> res;
	size_t index = 0;
	for(unsigned u : node)
	{
		if(u == v)
			res.push_back(weights[index]);
		++index;
	}
	return res;
}

template<typename WeightType>
WeightedGraph<WeightType>::WeightedGraph() : graph(), weights() {}

template<typename WeightType>
unsigned WeightedGraph<WeightType>::add(std::unique_ptr<Node> node)
{
	weights.push_back(std::vector<WeightType>());
	return graph.add(std::move(node));
}

template<typename WeightType>
unsigned WeightedGraph<WeightType>::add(std::unique_ptr<Node> node, const std::vector<unsigned>& friends, const std::vector<WeightType>& edgesWeights)
{
	weights.push_back(edgesWeights);
	return graph.add(std::move(node), friends);
}

template<typename WeightType>
template<typename NodeType>
unsigned WeightedGraph<WeightType>::add()
{
	return add(std::unique_ptr<Node>(new NodeType()));
}

template<typename WeightType>
template<typename NodeType>
unsigned WeightedGraph<WeightType>::add(const std::vector<unsigned>& friends, const std::vector<WeightType>& edgesWeights)
{
	return add(std::unique_ptr<Node>(new NodeType()), friends, edgesWeights);
}

template<typename WeightType>
void WeightedGraph<WeightType>::connect(unsigned v1, unsigned v2, WeightType weight)
{
	weights[v1].push_back(weight);
	return graph.connect(v1, v2);
}

template<typename WeightType>
bool WeightedGraph<WeightType>::areConnected(unsigned v1, unsigned v2) const
{
	return graph.areConnected(v1, v2);
}	

template<typename WeightType>
std::list<WeightType> WeightedGraph<WeightType>::getWeight(unsigned v1, unsigned v2) const
{
	return getNode(v1).getWeight(v2);
}	

template<typename WeightType>
WeightedNode<WeightType> WeightedGraph<WeightType>::getNode(unsigned v) const
{
	return WeightedNode<WeightType>(graph.getNode(v), weights[v]);
}

template<typename WeightType>
size_t WeightedGraph<WeightType>::size() const
{
	return graph.size();
}

template<typename WeightType>
const Graph& WeightedGraph<WeightType>::getBaseGraph()
{
	return graph;
}

}//namespace graph
