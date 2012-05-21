#ifndef STRONGLYCONNECTEDCOMPONENTSINFO_HPP
#define STRONGLYCONNECTEDCOMPONENTSINFO_HPP

class StronglyConnectedComponentsInfo {
public:
	StronglyConnectedComponentsInfo(size_t componentsNumber, const std::vector<size_t>& vertex2component):
		components(componentsNumber),vertex2component(vertex2component){}
	/*
	 Complexity: O(n) where n is number of vertices
	 */
	std::vector<std::vector<size_t>> allComponents() const {
		std::vector<std::vector<size_t>> ret(components);
		for(size_t i = 0; i < vertex2component.size(); ++i){
			ret[vertex2component[i]].push_back(i);
		}
		return ret;
	}
	/*
	 Complexity: O(n) where n is number of vertices
	 */
	std::vector<size_t> componentByID(size_t componentID) const {
		std::vector<size_t> ret;
		for(size_t i = 0; i < vertex2component.size(); ++i)
			if(componentID == vertex2component[i])
				ret.push_back(i);
		return ret;
	}
	
	std::vector<size_t> ids() const {
		return vertex2component;
	}
	
	size_t componentIDByVertex(size_t vertexID) const {
		return vertex2component[vertexID];
	}
	
	size_t getComponentsNumber() const {
		return components;
	}
private:
	size_t components;
	std::vector<size_t> vertex2component;
};

#endif /* STRONGLYCONNECTEDCOMPONENTSINFO_HPP */

