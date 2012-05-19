#pragma once

#include <vector>
#include <memory>

namespace graph
{

class NodeIterator
{
public:
	virtual NodeIterator& operator++() = 0;
	virtual NodeIterator& operator--() = 0;
	virtual unsigned operator*() const = 0;
	virtual bool operator==(const NodeIterator& it) const = 0;
	virtual bool operator!=(const NodeIterator& it) const = 0;
	virtual ~NodeIterator();
};

class IteratorWrapper
{
public:
	explicit IteratorWrapper(std::unique_ptr<NodeIterator> it);
	IteratorWrapper& operator++();
	IteratorWrapper& operator--();
	unsigned operator*() const;
	bool operator==(const IteratorWrapper& iw) const;
	bool operator!=(const IteratorWrapper& iw) const;
	std::unique_ptr<NodeIterator>& getBaseIterator();
	
private:
	std::unique_ptr<NodeIterator> base;
};

class Node
{
public:
	virtual void linkTo(unsigned v) = 0;
	virtual bool isConnected(unsigned v) const = 0;
	virtual std::vector<unsigned> getFriends() const = 0;
	virtual IteratorWrapper begin() const = 0;
	virtual IteratorWrapper end() const = 0;
	virtual ~Node();
};

class ListNodeIterator : public NodeIterator
{
public:
	explicit ListNodeIterator(std::vector<unsigned>::const_iterator it);
	virtual ListNodeIterator& operator++();
	virtual ListNodeIterator& operator--();
	virtual unsigned operator*() const;
	virtual bool operator==(const NodeIterator& it) const;
	virtual bool operator!=(const NodeIterator& it) const;
	
private:
	std::vector<unsigned>::const_iterator base;
};

class ListNode : public Node
{
public:
	virtual void linkTo(unsigned v);
	virtual bool isConnected(unsigned v) const;
	virtual std::vector<unsigned> getFriends() const;
	virtual IteratorWrapper begin() const;
	virtual IteratorWrapper end() const;
	
private:
	std::vector<unsigned> friends;
};

class TableNodeIterator : public NodeIterator
{
	typedef std::vector<bool>::const_iterator v_iter;

public:
	TableNodeIterator(v_iter it, v_iter vbegin, v_iter vend);
	virtual TableNodeIterator& operator++();
	virtual TableNodeIterator& operator--();
	virtual unsigned operator*() const;
	virtual bool operator==(const NodeIterator& it) const;
	virtual bool operator!=(const NodeIterator& it) const;
	
private:
	v_iter base, begin, end;
};

class TableNode : public Node
{
public:
	virtual void linkTo(unsigned v);
	virtual bool isConnected(unsigned v) const;
	virtual std::vector<unsigned> getFriends() const;
	virtual IteratorWrapper begin() const;
	virtual IteratorWrapper end() const;
	
private:
	std::vector<bool> friends;
};

class Graph
{
public:
	unsigned add(std::unique_ptr<Node> node);
	unsigned add(std::unique_ptr<Node> node, const std::vector<unsigned>& friends);
	void connect(unsigned v1, unsigned v2);
	bool areConnected(unsigned v1, unsigned v2) const;
	const Node& getNode(unsigned v) const;
	size_t size() const;
	
private:
	std::vector<std::unique_ptr<Node>> nodes;
};

}
