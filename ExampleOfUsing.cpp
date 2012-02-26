#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include "graph.h"

template<class TypeNameVer>
class MyVertex : public Vertex<TypeNameVer> {
public:
    virtual void add_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.push_back(nameVer);
    }
    virtual void delete_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.erase(find(neighbours.begin(), neighbours.end(), nameVer));
    }
    virtual std::vector<TypeNameVer>* list_neighbour() const
    {
        std::vector<TypeNameVer>* Vec = new std::vector<TypeNameVer>;
        Vec->resize(neighbours.size());
        std::copy(neighbours.begin(), neighbours.end(), Vec->begin());
        return Vec;
    }
    virtual size_t degree() const
    {
        return neighbours.size();
    }
    virtual ~MyVertex()
    {
        neighbours.clear();
    }
private:
    std::vector<TypeNameVer> neighbours;  // ?
};

void dfs(int vertex, Graph<int, Vertex<int> >& graph, std::vector<int>& mark)
{
    mark[vertex] = 1;
    std::vector<int>* neighbours;
    neighbours = graph.list_neighbour(vertex);
    for(size_t i = 0; i < neighbours->size(); i++)
    {
        if(mark[(*neighbours)[i]] != 1)
        {
            dfs((*neighbours)[i], graph, mark);
        }
    }
}

int main()
{
    Graph<int, Vertex<int> > graph;

    graph.add_vertex(1, MyVertex<int>());
    graph.add_vertex(2);
    graph.add_vertex(3, MyVertex<int>());
    graph.add_vertex(4);

    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(1, 4);
    graph.add_edge(2, 1);

    std::vector<int> mark;
    mark.resize(graph.size());
    dfs(2, graph, mark);
    std::cout << "component of 2:" << std::endl;
    for(int i = 1; i <= 4; i++)
    {
        if(mark[i])
        {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    return 0;
}
