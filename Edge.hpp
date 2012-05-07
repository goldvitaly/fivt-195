#ifndef VERTEX_HPP
#define VERTEX_HPP
#include <vector>
using std::size_t;
struct NoWeight{
	
};

template<typename W>
class Edge{
public:
	typedef W Weight;
	size_t id;
	Weight weight;
	explicit Edge(size_t id,const Weight& weight = Weight()):id(id), weight(weight){}
	bool operator < (const Edge& other) const {
		return id < other.id;
	}
	
	bool operator == (const Edge& other) const {
		return id == other.id;
	}
};

template<>
class Edge<NoWeight>{
public:
	typedef NoWeight Weight;
	size_t id;
	explicit Edge(size_t id):id(id){}
	Edge(size_t id, const NoWeight&):id(id){}
	bool operator < (const Edge& other) const {
		return id < other.id;
	}
	bool operator == (const Edge& other) const {
		return id == other.id;
	}
};

#endif /* VERTEX_HPP */

