#ifndef ALLSHORTESTPATH_HPP
#define ALLSHORTESTPATH_HPP
#include "Graph.hpp"
#include "Path.hpp"
#include "ShortestPathsInfo.hpp"
#include <boost/optional.hpp>
#include <set>
#include <vector>

template<typename Weight, typename Length = Weight, typename CalcLength = std::plus<Length> , typename CompLength = std::less<Length> >
class AllShortestPaths{
	typedef std::vector< std::vector< boost::optional<Length> > > LengthMatrix;
public: 
	explicit AllShortestPaths(const Graph<Weight>& graph, const CalcLength& calcLength = CalcLength(), const CompLength& compLength = CompLength())
		:graph(graph), calcLength(calcLength), compLength(compLength){}
	
		LengthMatrix calculate(){
			LengthMatrix res(graph.size(), std::vector< boost::optional<Length> >(graph.size()));
			for(size_t from=0; from < graph.size(); ++from){
				for(const Vertex<Weight>& to: graph.getIncidents(from)){
					res[from][to.id] = Length(to.weight);
				}
				res[from][from] = Length();
			}
			
			for (size_t k=0; k < graph.size(); ++k)
				for (size_t i=0; i < graph.size(); ++i)
					for (size_t j=0; j < graph.size(); ++j){
						if(res[i][k] && res[k][j]){
							Length len = calcLength(*res[i][k], *res[k][j]);
							if(!res[i][j] || compLength(len, *res[i][j]))
								res[i][j] = std::move(len);
						}
					}
			return res;
		}
private:
	const Graph<Weight>& graph;
	CalcLength calcLength;
	CompLength compLength;
};

#endif /* ALLSHORTESTPATHS_HPP */

