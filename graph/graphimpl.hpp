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
		explicit Edge(unsigned int to): to(to) {};
		bool operator < (const Edge& rhs) const {
			return to < rhs.to;
		}
		bool operator == (const Edge& rhs) const {
			return to == rhs.to;
		}
		bool operator != (const Edge& rhs) const {
			return to != rhs.to;
		}
};

template <class EdgeInfo> 
class Graph
{
	private:
		class IteratorWrapper;
	public:
		typedef Edge<EdgeInfo> edge_type;
		typedef IteratorWrapper iterator;		
		void add_edge(unsigned int from, unsigned int to, EdgeInfo edge_info) { edges_count += this->get(from).add(edge_type(to, edge_info)); };
		void del_edge(unsigned int from, unsigned int to, EdgeInfo edge_info) { edges_count -= this->get(from).del(edge_type(to, edge_info)); };
		bool has_edge(unsigned int from, unsigned int to) const { return this->get(from).has(edge_type(to)); };
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
				virtual iterator begin() const = 0;
				virtual iterator end() const = 0;
				virtual unsigned int size() const = 0;
				virtual bool add(edge_type) = 0;
				virtual bool del(edge_type) = 0;
				virtual bool has(edge_type) const = 0;
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
		class Iterator
		{
			public:
				virtual void next() = 0;
//				virtual void prev() = 0;
				virtual const edge_type& get() const = 0;
				virtual Iterator* clone() const = 0;
				virtual ~Iterator() {};
				virtual bool operator == (const Iterator& it) const = 0;
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
				
				const edge_type& operator * () const { return pointer_to_iterator->get(); }
				const edge_type* operator -> () const { return &pointer_to_iterator->get(); };
				IteratorWrapper& operator ++ ()	{ pointer_to_iterator->next(); return *this; }
				IteratorWrapper& operator ++ (int) { pointer_to_iterator->next(); return *this; }
//				IteratorWrapper& operator -- ()
//				{
//					pointer_to_iterator->prev();
//				}			
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
};


}; // namespace impl
}; // namespace graph


#endif //GRAPH_H
