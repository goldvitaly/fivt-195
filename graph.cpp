#include "graph.hpp"

#include <algorithm>

namespace graph
{

NodeIterator::~NodeIterator() {}

IteratorWrapper::IteratorWrapper(std::unique_ptr<NodeIterator> it)
{
	base = std::move(it);
}

IteratorWrapper& IteratorWrapper::operator++()
{
	++(*base);
	return *this;
}

IteratorWrapper& IteratorWrapper::operator--()
{
	--(*base);
	return *this;
}

unsigned IteratorWrapper::operator*() const
{
	return **base;
}

bool IteratorWrapper::operator==(const IteratorWrapper& iw) const
{
	return *base == *iw.base;
}

bool IteratorWrapper::operator!=(const IteratorWrapper& iw) const
{
	return *base != *iw.base;
}

std::unique_ptr<NodeIterator>& IteratorWrapper::getBaseIterator()
{
	return base;
}

Node::~Node() {}


ListNodeIterator::ListNodeIterator(std::vector<unsigned>::const_iterator it)
{
	base = it;
}

ListNodeIterator& ListNodeIterator::operator++()
{
	++base;
	return *this;
}

ListNodeIterator& ListNodeIterator::operator--()
{
	--base;
	return *this;
}

unsigned ListNodeIterator::operator*() const
{
	return *base;
}

bool ListNodeIterator::operator==(const NodeIterator& it) const
{
	return base == ((ListNodeIterator&)it).base;
}

bool ListNodeIterator::operator!=(const NodeIterator& it) const
{
	return base != ((ListNodeIterator&)it).base;
}		

void ListNode::linkTo(unsigned v)
{
	friends.push_back(v);
}

bool ListNode::isConnected(unsigned v) const
{
	return std::find(friends.begin(), friends.end(), v) != friends.end();
}

std::vector<unsigned> ListNode::getFriends() const
{
	return friends;
}

IteratorWrapper ListNode::begin() const
{
	return IteratorWrapper(std::unique_ptr<NodeIterator>(new ListNodeIterator(friends.begin())));
}

IteratorWrapper ListNode::end() const
{
	return IteratorWrapper(std::unique_ptr<NodeIterator>(new ListNodeIterator(friends.end())));
}

TableNodeIterator::TableNodeIterator(v_iter it, v_iter vbegin, v_iter vend)
{
	base = it;
	begin = vbegin;
	end = vend;
	for(;base != end && !(*base); ++base);
}

TableNodeIterator& TableNodeIterator::operator++()
{
	++base;
	for(;base != end && !(*base); ++base);
	return *this;
}

TableNodeIterator& TableNodeIterator::operator--()
{
	--base;
	for(;base != begin && !(*base); --base);
	return *this;
}

unsigned TableNodeIterator::operator*() const
{
	return base - begin;
}

bool TableNodeIterator::operator==(const NodeIterator& it) const
{
	return base == ((TableNodeIterator&)it).base;
}

bool TableNodeIterator::operator!=(const NodeIterator& it) const
{
	return base != ((TableNodeIterator&)it).base;
}

void TableNode::linkTo(unsigned v)
{
	if(friends.size() <= v)
		friends.resize(v + 1);
	friends[v] = true;
}

bool TableNode::isConnected(unsigned v) const
{
	if(friends.size() <= v)
		return false;
	return friends[v];
}

std::vector<unsigned> TableNode::getFriends() const
{
	std::vector<unsigned> result;
	for(auto it : friends)
		if(it)
			result.push_back(it);
	return result;
}

IteratorWrapper TableNode::begin() const
{
	return IteratorWrapper(std::unique_ptr<NodeIterator>(new TableNodeIterator(friends.begin(), friends.begin(), friends.end())));
}

IteratorWrapper TableNode::end() const
{
	return IteratorWrapper(std::unique_ptr<NodeIterator>(new TableNodeIterator(friends.end(), friends.begin(), friends.end())));
}

unsigned Graph::add(std::unique_ptr<Node> node)
{
	nodes.push_back(std::move(node));
	return nodes.size() - 1;
}

unsigned Graph::add(std::unique_ptr<Node> node, const std::vector<unsigned>& friends)
{
	for(auto v : friends)
		node->linkTo(v);
	nodes.push_back(std::move(node));
	return nodes.size() - 1;
}

void Graph::connect(unsigned v1, unsigned v2)
{
	nodes[v1]->linkTo(v2);
}

bool Graph::areConnected(unsigned v1, unsigned v2) const
{
	return nodes[v1]->isConnected(v2);
}

const Node& Graph::getNode(unsigned v) const
{
	return *nodes[v];
}

size_t Graph::size() const
{
	return nodes.size();
}

}//namespace graph
