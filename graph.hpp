#include <vector>
#include <map>

template<typename IdType, typename DataType>
class Vertex
{
	public:
		typedef IdType vertex_id_t;
		
		Vertex(IdType& vertexId, DataType& vertexData) { id = vertexId; data = vertexData; }
		DataType getData() { return data; }
	
	private:
		IdType id; 
		DataType data;
};

template<typename VertexType, typename EdgeDataType>
class Graph
{
	public:
		typedef typename VertexType::vertex_id_t vertex_id_t;
		
		void addVertex(VertexType& vertex);
		void link(vertex_id_t& vertex1, vertex_id_t& vertex2, EdgeDataType edgeData);
		std::vector<VertexType&> getLinked(vertex_id_t& vertex);
		void removeVertex(vertex_id_t& vertex);
		
	private:
		std::vector<VertexType> vertices;
		std::map<vertex_id_t, std::vector<vertex_id_t> > links;
		std::map<vertex_id_t, EdgeDataType> edges;
};
