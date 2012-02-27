#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include "graph.h"
#include <ctime>


template<class TypeNameVer>
class MyVertex : public Vertex<TypeNameVer> {
public:
    void add_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.push_back(nameVer);
    }
    void delete_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.erase(find(neighbours.begin(), neighbours.end(), nameVer));
    }
    std::vector<TypeNameVer>* list_neighbour() const
    {
        std::vector<TypeNameVer>* Vec = new std::vector<TypeNameVer>;
        Vec->resize(neighbours.size());
        std::copy(neighbours.begin(), neighbours.end(), Vec->begin());
        return Vec;
    }
    size_t degree() const
    {
        return neighbours.size();
    }
    ~MyVertex()
    {
        neighbours.clear();
    }
private:
    std::vector<TypeNameVer> neighbours;
};

void make_graph(Graph<int, Vertex<int> >& graph, int numVer)
{
    srand(time(NULL));
    for(int i = 0; i < numVer; i++)
        graph.add_vertex(i, MyVertex<int>());
    for(int i = 0; i < numVer; i++)
    {
        int in_ver = rand() % 100;
        int out_ver = rand() % 100;
        graph.add_edge(out_ver, in_ver);
    }
}

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
    delete neighbours;
}

int main()
{
    Graph<int, Vertex<int> > graph;
    int numVer = 100;
    make_graph(graph, numVer);

    std::vector<int> mark;
    mark.resize(numVer);
    dfs(1, graph, mark);
    std::cout << "component of 1:" << std::endl;
    for(int i = 1; i <= numVer; i++)
    {
        if(mark[i])
        {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    return 0;
}
