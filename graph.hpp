#include <vector>
#include <map>

template<typename IdType>
class Vertex
{
	public:
		typedef IdType vertex_id_t;
		
		Vertex() {}
		Vertex(IdType& vertexId) { id = vertexId; }
		IdType& getId() { return id; }
		
	private:
		IdType id;
};

template<typename IdType, typename DataType>
class DataVertex : public Vertex<IdType>
{
	public:
		DataVertex(IdType& vertexId, DataType& vertexData) : Vertex<IdType>(vertexId) { data = vertexData; }
		DataType getData() { return data; }
	
	private:
		DataType data;
};

template<typename VertexType, typename EdgeDataType>
class Graph
{
	public:
		typedef typename VertexType::vertex_id_t vertex_id_t;
		typedef typename std::map<vertex_id_t, std::vector<std::pair<vertex_id_t, EdgeDataType > > > links_map_t;
		typedef typename std::vector<std::pair<vertex_id_t, EdgeDataType > > links_list_t;
		typedef typename links_list_t::iterator links_iterator_t;
		typedef typename links_map_t::iterator links_map_iterator_t;
		
		void addVertex(VertexType& vertex);
		void link(vertex_id_t& vertex1, vertex_id_t& vertex2, EdgeDataType edgeData);
		links_list_t getLinksList(vertex_id_t& vertex);
		links_iterator_t getLinksIterator(vertex_id_t& vertex);
		void removeVertex(vertex_id_t& vertex);
		
	private:
		std::vector<VertexType> vertices;
		std::map<vertex_id_t, std::vector<std::pair<vertex_id_t, EdgeDataType > > > links;
};

template<typename VertexType, typename EdgeDataType>
void Graph<VertexType, EdgeDataType>::addVertex(VertexType& vertex)
{
	vertices.push_back(vertex);
	links[vertex.getId()] = std::vector<std::pair<vertex_id_t, EdgeDataType > >();
}

template<typename VertexType, typename EdgeDataType>
void Graph<VertexType, EdgeDataType>::link(vertex_id_t& vertex1, vertex_id_t& vertex2, EdgeDataType edgeData)
{
	links_map_iterator_t it = links.find(vertex1);
	std::pair<vertex_id_t&, EdgeDataType> ins(vertex2, edgeData);
	it->second.push_back(ins);
}

template<typename VertexType, typename EdgeDataType>
typename Graph<VertexType, EdgeDataType>::links_list_t Graph<VertexType, EdgeDataType>::getLinksList(vertex_id_t& vertex)
{
	links_map_iterator_t it = links.find(vertex);
	return it->second;
}

template<typename VertexType, typename EdgeDataType>
typename Graph<VertexType, EdgeDataType>::links_iterator_t Graph<VertexType, EdgeDataType>::getLinksIterator(vertex_id_t& vertex)
{
	return getLinksList(vertex);
}