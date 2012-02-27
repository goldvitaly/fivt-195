#ifndef VECTORINCIDENTS_HPP
#define VECTORINCIDENTS_HPP
#include <vector>
#include "Incidents.hpp"
class VectorIncidents : public Incidents {
public:
	class Iterator : public Incidents::Iterator {
	public:
		Iterator(const std::vector<size_t>& v){
			cur = v.begin();
			end = v.end();
		}
	
		virtual bool is_valid() const {
			return cur != end;
		}
		virtual void increase(){
			++cur;
		}
		virtual size_t value() const {
			return *cur;
		}
	private:
		std::vector<size_t>::const_iterator cur, end;
	};

	VectorIncidents(){
		
	}
	VectorIncidents(std::vector<size_t> v): incidents(v) {
		
	}
	
	Incidents::Iterator* createIterator() const {
		return new VectorIncidents::Iterator(incidents);
	}
	
	size_t count() const {
		return incidents.size();
	}
	
	virtual void addEdge(size_t to){
		incidents.push_back(to);
	}
	
	virtual void removeEdge(size_t to){
		for(std::vector<size_t>::iterator it=incidents.begin(); it!=incidents.end(); ++it){
			if(*it == to){
				incidents.erase(it);
				break;
			}
		}
	}
	
	virtual bool checkEdge(size_t to) const {
		for(std::vector<size_t>::const_iterator it=incidents.begin(); it!=incidents.end(); ++it){
			if(*it == to){
				return true;
			}
		}
		return false;
	}
	
private:
	std::vector<size_t> incidents;
};
#endif /* VECTORINCIDENTS_HPP */

