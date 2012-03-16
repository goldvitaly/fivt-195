#ifndef STRONGLYCONNECTEDCOMPONENTSINFO_HPP
#define STRONGLYCONNECTEDCOMPONENTSINFO_HPP

class StronglyConnectedComponentsInfo {
public:
	StronglyConnectedComponentsInfo(size_t componentCount, const std::vector<size_t>& vertex2component):
		components(componentCount),vertex2component(vertex2component){}
	/*
	 Complexity: O(n) where n is number of vertices
	 */
	std::vector<std::vector<size_t>> allComponents(){
		std::vector<std::vector<size_t>> ret(components);
		for(size_t i = 0; i < vertex2component.size(); ++i){
			ret[vertex2component[i]].push_back(i);
		}
		return ret;
	}
	/*
	 Complexity: O(n) where n is number of vertices
	 */
	std::vector<size_t> componentByID(size_t componentID){
		std::vector<size_t> ret;
		for(size_t i = 0; i < vertex2component.size(); ++i)
			if(componentID == vertex2component[i])
				ret.push_back(i);
		return ret;
	}
	
	std::vector<size_t> ids(){
		return vertex2component;
	}
	
	size_t componentIDByVertex(size_t vertexID){
		return vertex2component[vertexID];
	}
	
	size_t getComponentsNumber(){
		return components;
	}
private:
	size_t components;
	std::vector<size_t> vertex2component;
};

#endif /* STRONGLYCONNECTEDCOMPONENTSINFO_HPP */

