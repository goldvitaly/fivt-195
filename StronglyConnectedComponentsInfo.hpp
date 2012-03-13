#ifndef STRONGLYCONNECTEDCOMPONENTSINFO_HPP
#define STRONGLYCONNECTEDCOMPONENTSINFO_HPP

class StronglyConnectedComponentsInfo {
public:
	StronglyConnectedComponentsInfo(size_t componentCount, const std::vector<size_t>& vertex2component):
		components(componentCount),data(vertex2component){}
	std::vector<std::vector<size_t>> getAllComponents(){
		std::vector<std::vector<size_t>> ret(components);
		for(size_t i = 0; i < data.size(); ++i){
			ret[data[i]].push_back(i);
		}
		return ret;
	}
	
	std::vector<size_t> getComponentByID(size_t componentID){
		std::vector<size_t> ret;
		for(size_t i = 0; i < data.size(); ++i)
			if(componentID == data[i])
				ret.push_back(i);
		return ret;
	}
	
	std::vector<size_t> getIds(){
		return data;
	}
	
	size_t getComponentIDByVertex(size_t vertexID){
		return data[vertexID];
	}
	
	size_t getComponentsCount(){
		return components;
	}
private:
	size_t components;
	std::vector<size_t> data;
};

#endif /* STRONGLYCONNECTEDCOMPONENTSINFO_HPP */

