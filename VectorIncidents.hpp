#ifndef VECTORINCIDENTS_HPP
#define VECTORINCIDENTS_HPP
#include "ContainerBaseIterator.hpp"
#include "Incidents.hpp"
#include <algorithm>
#include <typeinfo>
#include <vector>
template <typename Weight>
class VectorIncidents : public Incidents<Weight> {
	typedef Vertex<Weight> CurVertex;
	typedef ContainerBaseIterator<std::vector<Vertex<Weight> > > BaseIterator;
public:
	VectorIncidents(){}
	explicit VectorIncidents(const std::vector<CurVertex>& v): incidents(v) {}
	
	virtual typename Incidents<Weight>::Iterator begin() const{
		return typename Incidents<Weight>::Iterator(std::unique_ptr<typename Incidents<Weight>::BaseIterator>(new BaseIterator(incidents.begin())));
	}
	virtual typename Incidents<Weight>::Iterator end() const{
		return typename Incidents<Weight>::Iterator(std::unique_ptr<typename Incidents<Weight>::BaseIterator>(new BaseIterator(incidents.end())));
	}
	
	size_t size() const {
		return incidents.size();
	}
	
	virtual void addEdge(size_t to, const Weight& weight = Weight()){
		incidents.push_back(CurVertex(to,weight));
	}
	
	virtual void removeEdge(size_t to){
		typename std::vector<CurVertex>::iterator founded=std::find(incidents.begin(),incidents.end(), CurVertex(to));
		if(founded !=incidents.end())
			incidents.erase(founded);
	}
	
	virtual bool checkEdge(size_t to) const {
		return std::find(incidents.begin(),incidents.end(), CurVertex(to)) != incidents.end();
	}
	
private:
	std::vector<CurVertex> incidents;
};
#endif /* VECTORINCIDENTS_HPP */