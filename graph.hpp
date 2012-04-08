#pragma once

#include <vector>
#include <algorithm>
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
		virtual ~NodeIterator() {}
};

class IteratorWrapper
{
	public:
		explicit IteratorWrapper(std::unique_ptr<NodeIterator> it)
		{
			base = std::move(it);
		}
		
		IteratorWrapper& operator++()
		{
			++(*base);
			return *this;
		}
		
		IteratorWrapper& operator--()
		{
			--(*base);
			return *this;
		}
		
		unsigned operator*() const
		{
			return **base;
		}
		
		bool operator==(const IteratorWrapper& iw) const
		{
			return *base == *iw.base;
		}
		
		bool operator!=(const IteratorWrapper& iw) const
		{
			return *base != *iw.base;
		}
		
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
		virtual ~Node() {}
};

class ListNodeIterator : public NodeIterator
{
	public:
		explicit ListNodeIterator(std::vector<unsigned>::const_iterator it)
		{
			base = it;
		}
		
		virtual ListNodeIterator& operator++()
		{
			++base;
			return *this;
		}
		
		virtual ListNodeIterator& operator--()
		{
			--base;
			return *this;
		}
		
		virtual unsigned operator*() const
		{
			return *base;
		}
		
		virtual bool operator==(const NodeIterator& it) const
		{
			return base == ((ListNodeIterator&)it).base;
		}
		
		virtual bool operator!=(const NodeIterator& it) const
		{
			return base != ((ListNodeIterator&)it).base;
		}
		
	private:
		std::vector<unsigned>::const_iterator base;
};

class ListNode : public Node
{
	public:
		virtual void linkTo(unsigned v)
		{
			friends.push_back(v);
		}
		
		virtual bool isConnected(unsigned v) const
		{
			return std::find(friends.begin(), friends.end(), v) != friends.end();
		}
		
		virtual std::vector<unsigned> getFriends() const
		{
			return friends;
		}
		
		virtual IteratorWrapper begin() const
		{
			return IteratorWrapper(std::unique_ptr<NodeIterator>(new ListNodeIterator(friends.begin())));
		}
		
		virtual IteratorWrapper end() const
		{
			return IteratorWrapper(std::unique_ptr<NodeIterator>(new ListNodeIterator(friends.end())));
		}
		
		
	private:
		std::vector<unsigned> friends;
};

class TableNodeIterator : public NodeIterator
{
	typedef std::vector<bool>::const_iterator v_iter;
	
	public:
		TableNodeIterator(v_iter it, v_iter vbegin, v_iter vend)
		{
			base = it;
			begin = vbegin;
			end = vend;
			for(;base != end && !(*base); ++base);
		}
		
		virtual TableNodeIterator& operator++()
		{
			++base;
			for(;base != end && !(*base); ++base);
			return *this;
		}
		
		virtual TableNodeIterator& operator--()
		{
			--base;
			for(;base != begin && !(*base); --base);
			return *this;
		}
		
		virtual unsigned operator*() const
		{
			return base - begin;
		}
		
		virtual bool operator==(const NodeIterator& it) const
		{
			return base == ((TableNodeIterator&)it).base;
		}
		
		virtual bool operator!=(const NodeIterator& it) const
		{
			return base != ((TableNodeIterator&)it).base;
		}
		
	private:
		v_iter base, begin, end;
};

class TableNode : public Node
{
	public:
		virtual void linkTo(unsigned v)
		{
			if(friends.size() <= v)
				friends.resize(v + 1);
			friends[v] = true;
		}
		
		virtual bool isConnected(unsigned v) const
		{
			if(friends.size() <= v)
				return false;
			return friends[v];
		}
		
		virtual std::vector<unsigned> getFriends() const
		{
			std::vector<unsigned> ret;
			for(auto it : friends)
				if(it)
					ret.push_back(it);
			return ret;
		}
		
		virtual IteratorWrapper begin() const
		{
			return IteratorWrapper(std::unique_ptr<NodeIterator>(new TableNodeIterator(friends.begin(), friends.begin(), friends.end())));
		}
		
		virtual IteratorWrapper end() const
		{
			return IteratorWrapper(std::unique_ptr<NodeIterator>(new TableNodeIterator(friends.end(), friends.begin(), friends.end())));
		}
		
	private:
		std::vector<bool> friends;
};

class Graph
{
	public:
		unsigned add(std::unique_ptr<Node> node)
		{
			nodes.push_back(std::move(node));
			return nodes.size() - 1;
		}
		
		unsigned add(std::unique_ptr<Node> node, const std::vector<unsigned>& friends)
		{
			for(auto v : friends)
				node->linkTo(v);
			nodes.push_back(std::move(node));
			return nodes.size() - 1;
		}
		
		void connect(unsigned v1, unsigned v2)
		{
			nodes[v1]->linkTo(v2);
		}
		
		bool areConnected(unsigned v1, unsigned v2) const
		{
			return nodes[v1]->isConnected(v2);
		}		
		
		const Node& getNode(unsigned v) const
		{
			return *nodes[v];
		}
		
		size_t size() const
		{
			return nodes.size();
		}
		
	private:
		std::vector<std::unique_ptr<Node>> nodes;
};

}
