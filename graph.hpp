#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <cstdlib>

#include "macro.hpp"

/**
 * @brief	Abstract virtual class representing graph's vertex's incidence list
 */
class Incidence
{
public:
	virtual void add_edge(size_t) = 0;
	virtual void delete_edge(size_t) = 0;
	virtual bool check_edge(size_t) const = 0;
	
	class Iterator_base
	{
	public:
		virtual size_t operator* () = 0;
		virtual Iterator_base& operator++ () = 0;
		virtual Iterator_base& operator++ (int) = 0;
		virtual ~Iterator_base ()
		{
		}
	};
	class Iterator
	{
	public:
		Iterator_base *iterator;

		Iterator(Iterator_base *iterator_)
		{
			iterator = iterator_;
		}
		
		size_t operator*()
		{
			return **iterator;
		}
		Iterator operator++ ()
		{
			++(*iterator);
			return *this;
		}
		Iterator operator++ (int)
		{
			(*iterator)++;
			return *this;
		}
		
		~Iterator ()
		{
			delete iterator;
		}
	};
	
	virtual Iterator begin() const = 0;
	virtual Iterator end() const = 0;
};


/**
 * @brief	Data structure representing an unoriented graph
 * 
 * @todo 	Add any flags and be able to handle oriented graphs
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
	
	Incidence* operator[] (size_t v)
	{
		return incident[v];
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
		incident[to]->add_edge(from);
	}
	void delete_edge(size_t from, size_t to)
	{
		m_assert(from < incident.size() && to < incident.size(), vertex number out of bound);
		incident[from]->delete_edge(to);
		incident[to]->delete_edge(to);
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
		std::vector<size_t>::const_iterator iterator;
		
	public:
		#warning todo
		///TODO make all iterators constant
		VectorIterator_base (std::vector<size_t>::iterator iterator_)
		{
			iterator = iterator_;
		}
		VectorIterator_base (std::vector<size_t>::const_iterator iterator_)
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
		VectorIterator_base& operator++ (int)
		{
			iterator++;
			return *this;
		}
		
		~VectorIterator_base ()
		{
		}
	};
	
	Iterator begin() const
	{
		return Iterator(new VectorIterator_base(incident.begin()));
	}
	Iterator end() const
	{
		return Iterator(new VectorIterator_base(incident.end()));
	}
};

#endif
