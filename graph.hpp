#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <typeinfo>

#include "incidence.hpp"
#include "macro.hpp"


/**
 * @brief	Data structure representing an unoriented graphs
 */
class Graph
{
private:
	std::vector<Incidence*> incident;

public:
	Graph()
	{
	}
	
	~Graph()
	{
		for (auto it: incident)
			delete it;
	}
	
	///@todo add const
	Incidence& operator[] (size_t v)
	{
		return *incident[v];
	}
	
	template<typename T>
	void add_vertex()
	{
		incident.push_back(new T);
	}
	void add_edge(size_t from, size_t to)
	{
		m_assert(from < incident.size() && to < incident.size(), vertex number out of bound);
		incident[from]->add_edge(to);
	}
	void add_undirected_edge(size_t from, size_t to)
	{
		add_edge(from, to);
		add_edge(to, from);
	}
	void delete_edge(size_t from, size_t to)
	{
		m_assert(from < incident.size() && to < incident.size(), vertex number out of bound);
		incident[from]->delete_edge(to);
	}
	void delete_undirected_edge(size_t from, size_t to)
	{
		delete_edge(from, to);
		delete_edge(to, from);
	}
};

class IncidenceVector: public Incidence
{
private:
	std::vector<size_t> incident;

public:
	void add_edge(size_t to)
	{
		incident.push_back(to);
	}
	void delete_edge(size_t to)
	{
		incident.resize(std::remove(incident.begin(), incident.end(), to) - incident.begin());
	}
	bool check_edge(size_t to) const
	{
		return find(incident.begin(), incident.end(), to) != incident.end();
	}
	
	~IncidenceVector()
	{
	}
	
	class VectorIterator_base: public Iterator_base
	{
	private:
		std::vector<size_t>::iterator iterator;
		
	public:
		#warning todo
		///TODO make all iterators constant
		VectorIterator_base (std::vector<size_t>::iterator iterator_)
		{
			iterator = iterator_;
		}
		
		size_t operator* ()
		{
			return *iterator;
		}
		VectorIterator_base& operator++ ()
		{
			++iterator;
			return *this;
		}
		bool operator != (const Iterator_base& it) const
		{
			//std::cerr << typeid(it).name() << std::endl;
			return false;
			//return dynamic_cast<const VectorIterator_base&>(it).iterator != iterator;
		}
		~VectorIterator_base() {}
	};
	
	Iterator begin()
	{
		return Iterator(new VectorIterator_base(std::begin(incident)));
	}
	Iterator end()
	{
		return Iterator(new VectorIterator_base(std::end(incident)));
	}
};

#endif
