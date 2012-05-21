#ifndef GRAPH_IMPLEMENTATION_H
#define GRAPH_IMPLEMENTATION_H

#include <vector>
#include <stddef.h>
#include <utility>
#include <memory>
#include <iterator>

#include <boost/optional.hpp>

namespace graph
{
namespace tag
{
	class no_info {};
}

namespace impl
{

template <class EdgeInfo>
class Edge
{
	public:
		unsigned int to;
		EdgeInfo info;
		Edge(unsigned int to, EdgeInfo info): to(to), info(info) {};
};

template <class Edge> 
class EdgeComparator
{
	public:
		bool operator()(const Edge& lhs, const Edge& rhs) 
		{
			return lhs.to < rhs.to;
		}
		bool operator()(const Edge& lhs, unsigned int rhs)
		{
			return lhs.to < rhs;
		}
		bool operator()(unsigned int lhs, const Edge& rhs)
		{
			return lhs < rhs.to;
		}
};

template <class Edge>
class EdgeIsEqualPred
{
	private:
		int value_to_compare;
	public:
		EdgeIsEqualPred(int value): value_to_compare(value)
		{
		}
		bool operator() (const Edge& edge)
		{
			return edge.to == value_to_compare;
		}
};


// EdgeInfo should be copyable

template <class EdgeInfo> 
class Graph
{
	private:
		class IteratorWrapper;
		class ConstIteratorWrapper;
	public:
		typedef Edge<EdgeInfo> edge_type;
		typedef IteratorWrapper iterator;		
		typedef ConstIteratorWrapper const_iterator;		
		void add_edge(unsigned int from, unsigned int to, EdgeInfo edge_info) { edges_count += this->get(from).add(edge_type(to, edge_info)); };
		void del_edge(unsigned int from, unsigned int to, EdgeInfo edge_info) { edges_count -= this->get(from).del(to); };
		bool has_edge(unsigned int from, unsigned int to) const { return this->get(from).has(to); };
//		bool has_edge(unsigned int from, unsigned int to, EdgeInfo edge_info) const { return this->get(from).has(edge_type(to, edge_info)); };
		const EdgeInfo& get_edge(unsigned int from, unsigned int to) const { return this->get(from).get(to).info; }
		unsigned int size() const 
		{ 
			return vertices.size(); 
		}
		unsigned int edges() const
		{ 
			return edges_count; 
		}
		Graph(unsigned int vertex_number): vertices(vertex_number) 
		{
			edges_count = 0;
		}
		Graph& operator = (const Graph& rhs)
		{
			edges_count = rhs.edges_count;
			vertices.resize(rhs.size());
			for (unsigned int i = 0; i < rhs.size(); i ++)
				vertices[i].reset(rhs.vertices[i]->clone());
			return *this;
		}
		explicit Graph(const Graph& G)
		{
			*this = G;
		}
		virtual ~Graph() {}; 
		class Vertex
		{
			public:
				virtual iterator begin() = 0;
				virtual iterator end() = 0;
				virtual const_iterator begin() const = 0;
				virtual const_iterator end() const = 0;
				virtual unsigned int size() const = 0;
				virtual bool add(edge_type) = 0;
				virtual bool del(unsigned int) = 0;
				virtual bool has(unsigned int) const = 0;
				virtual const edge_type& get(unsigned int) const = 0;
				virtual Vertex* clone() const = 0;
				virtual ~Vertex() {};
		};
		void reset(unsigned int vertex, Vertex* realization)
		{
			if (vertices[vertex])
			{
				for (iterator it = vertices[vertex]->begin(); it != vertices[vertex]->end(); it ++)
					realization->add(*it);
			}
			vertices[vertex].reset(realization);
		}
		const Vertex& operator [](unsigned int vertex) const { return *vertices[vertex]; };
		Vertex& operator [](unsigned int vertex) { return *vertices[vertex]; };
		class Iterator
		{
			public:
				virtual void next() = 0;
				virtual edge_type& get() = 0;
				virtual Iterator* clone() const = 0;
				virtual ~Iterator() {};
				virtual bool operator == (const Iterator& it) const = 0;
		};
		class ConstIterator
		{
			public:
				virtual void next() = 0;
				virtual const edge_type& get() const = 0;
				virtual ConstIterator* clone() const = 0;
				virtual ~ConstIterator() {};
				virtual bool operator == (const ConstIterator& it) const = 0;
		};
	protected:
		std::vector < std::unique_ptr <Vertex> > vertices;
		Vertex& get(unsigned int vertex) { return *vertices[vertex]; };
		const Vertex& get(unsigned int vertex) const { return *vertices[vertex]; };
	private:
		unsigned int edges_count;
		class IteratorWrapper
		{
			private:
				std::unique_ptr<Iterator> pointer_to_iterator;
			public:
				// typedefs for enabling iterator_traits for this iterator
				typedef std::forward_iterator_tag iterator_category;
				typedef edge_type value_type;
				typedef ptrdiff_t difference_type;
				typedef edge_type& reference;
				typedef edge_type* pointer;

				edge_type& operator * () { return pointer_to_iterator->get(); }
				edge_type* operator -> () { return &pointer_to_iterator->get(); }

				edge_type* get_pointer() { return &pointer_to_iterator->get(); }

				IteratorWrapper& operator ++ ()	{ pointer_to_iterator->next(); return *this; }
				IteratorWrapper& operator ++ (int) { pointer_to_iterator->next(); return *this; }
				IteratorWrapper& operator = (const IteratorWrapper& it)
				{
					pointer_to_iterator.reset(it.pointer_to_iterator->clone());
					return *this;
				}
				bool operator == (const IteratorWrapper& it)
				{
					if (!pointer_to_iterator && !it.pointer_to_iterator)
						return true;
					if (!(pointer_to_iterator && it.pointer_to_iterator))
						return false;
					return *pointer_to_iterator == *it.pointer_to_iterator;
				}
				bool operator != (const IteratorWrapper& it) { return !(*this == it); }
				IteratorWrapper(const IteratorWrapper& it) { *this = it; }
				IteratorWrapper(Iterator* pointer): pointer_to_iterator(pointer) {};
				~IteratorWrapper() {};
		};

		class ConstIteratorWrapper
		{
			private:
				std::unique_ptr<ConstIterator> pointer_to_iterator;
			public:
				// typedefs for enabling iterator_traits for this iterator
				typedef std::forward_iterator_tag iterator_category;
				typedef const edge_type value_type;
				typedef ptrdiff_t difference_type;
				typedef const edge_type& reference;
				typedef const edge_type* pointer;

				const edge_type& operator * () const { return pointer_to_iterator->get(); }
				const edge_type* operator -> () const { return &pointer_to_iterator->get(); }

				const edge_type* get_pointer() { return &pointer_to_iterator->get(); }

				ConstIteratorWrapper& operator ++ ()	{ pointer_to_iterator->next(); return *this; }
				ConstIteratorWrapper& operator ++ (int) { pointer_to_iterator->next(); return *this; }
				ConstIteratorWrapper& operator = (const ConstIteratorWrapper& it)
				{
					pointer_to_iterator.reset(it.pointer_to_iterator->clone());
					return *this;
				}
				bool operator == (const ConstIteratorWrapper& it)
				{
					if (!pointer_to_iterator && !it.pointer_to_iterator)
						return true;
					if (!(pointer_to_iterator && it.pointer_to_iterator))
						return false;
					return *pointer_to_iterator == *it.pointer_to_iterator;
				}
				bool operator != (const ConstIteratorWrapper& it) { return !(*this == it); }
				ConstIteratorWrapper(const ConstIteratorWrapper& it) { *this = it; }
				ConstIteratorWrapper(ConstIterator* pointer): pointer_to_iterator(pointer) {};
				~ConstIteratorWrapper() {};
		};
};


}; // namespace impl
}; // namespace graph


#endif //GRAPH_H
