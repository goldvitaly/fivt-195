#ifndef VECTORINCIDENTS_HPP
#define VECTORINCIDENTS_HPP
#include "ContainerBaseIterator.hpp"
#include "Incidents.hpp"
#include <algorithm>
#include <typeinfo>
#include <vector>
template <typename Weight>
class VectorIncidents : public Incidents<Weight> {
public:
	VectorIncidents(){}
	explicit VectorIncidents(const std::vector<Edge<Weight> >& v): incidents(v) {}
	
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
		incidents.push_back(Edge<Weight>(to,weight));
	}
	
	virtual void removeEdge(size_t to){
		typename std::vector<Edge<Weight> >::iterator founded=std::find(incidents.begin(),incidents.end(), Edge<Weight>(to));
		if(founded !=incidents.end())
			incidents.erase(founded);
	}
	
	virtual bool checkEdge(size_t to) const {
		return std::find(incidents.begin(),incidents.end(), Edge<Weight>(to)) != incidents.end();
	}
	
	virtual ~VectorIncidents(){}
private:
	typedef ContainerBaseIterator<std::vector<Edge<Weight> > > BaseIterator;
	std::vector<Edge<Weight> > incidents;
};
#endif /* VECTORINCIDENTS_HPP */