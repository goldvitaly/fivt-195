#ifndef VECTORINCIDENTS_HPP
#define VECTORINCIDENTS_HPP
#include "ContainerBaseIterator.hpp"
#include "Incidents.hpp"
#include <algorithm>
#include <typeinfo>
#include <vector>
class VectorIncidents : public Incidents {
public:
	VectorIncidents(){}
	explicit VectorIncidents(const std::vector<size_t>& v): incidents(v) {}
	
	virtual Iterator begin() const{
		return Iterator(std::unique_ptr<Incidents::BaseIterator>(new BaseIterator(incidents.begin())));
	}
	virtual Iterator end() const{
		return Iterator(std::unique_ptr<Incidents::BaseIterator>(new BaseIterator(incidents.end())));
	}
	
	size_t size() const {
		return incidents.size();
	}
	
	virtual void addEdge(size_t to){
		incidents.push_back(to);
	}
	
	virtual void removeEdge(size_t to){
		std::vector<size_t>::iterator founded=std::find(incidents.begin(),incidents.end(), to);
		if(founded !=incidents.end())
			incidents.erase(founded);
	}
	
	virtual bool checkEdge(size_t to) const {
		return std::find(incidents.begin(),incidents.end(), to) != incidents.end();
	}
	
private:
	typedef ContainerBaseIterator<std::vector<size_t>> BaseIterator;
	std::vector<size_t> incidents;
};
#endif /* VECTORINCIDENTS_HPP */