#ifndef VERTEX_HPP
#define VERTEX_HPP
#include <vector>
using std::size_t;
struct NoWeight{
	
};

template<typename W>
class Vertex{
public:
	typedef W Weight;
	size_t id;
	Weight weight;
	explicit Vertex(size_t id,const Weight& weight = Weight()):id(id), weight(weight){}
	bool operator < (const Vertex& other) const {
		return id < other.id;
	}
	
	bool operator == (const Vertex& other) const {
		return id == other.id;
	}
};

template<>
class Vertex<NoWeight>{
public:
	typedef NoWeight Weight;
	size_t id;
	explicit Vertex(size_t id):id(id){}
	Vertex(size_t id, const NoWeight&):id(id){}
	bool operator < (const Vertex& other) const {
		return id < other.id;
	}
	bool operator == (const Vertex& other) const {
		return id == other.id;
	}
};

#endif /* VERTEX_HPP */

