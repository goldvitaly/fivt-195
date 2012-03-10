#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

template<class TypeNameVer>
class Vertex {
public:
    virtual void add_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.insert(nameVer);
    }
    virtual void delete_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.erase(nameVer);
    }
    virtual std::vector<TypeNameVer> list_neighbour() const
    {
        std::vector<TypeNameVer> Vec;
        Vec.resize(neighbours.size());
        std::copy(neighbours.begin(), neighbours.end(), Vec.begin());
        return Vec;
    }
    virtual size_t degree() const
    {
        return neighbours.size();
    }
    virtual ~Vertex<TypeNameVer>()
    {
        neighbours.clear();
    }
private:
    std::set<TypeNameVer> neighbours;
};


template<class TypeNameVer, class StructVer>
class Graph {
public:
    void add_vertex(const TypeNameVer& nameVer, StructVer* structVer = new StructVer())
    {
        check_exist(nameVer, false);
        graphInv.insert(std::make_pair(nameVer, structVer));
        graph.insert(std::make_pair(nameVer, structVer));
    }
    void add_edge(const TypeNameVer& outVer, const TypeNameVer& inVer)
    {
        check_exist(outVer, true);
        check_exist(inVer, true);
        graph[outVer]->add_neighbour(inVer);
        graphInv[inVer]->add_neighbour(outVer);
    }
    void delete_edge(const TypeNameVer& outVer, const TypeNameVer& inVer)
    {
        check_exist(outVer, true);
        check_exist(inVer, true);
        graph[outVer]->delete_neighbour(inVer);
        graphInv[inVer]->delete_neighbour(outVer);
    }
    std::vector<TypeNameVer> list_neighbour(const TypeNameVer& nameVer)
    {
        check_exist(nameVer, true);
        return graph[nameVer]->list_neighbour();
    }
    size_t size() const
    {
        return graph.size();
    }
private:
    std::map<TypeNameVer, StructVer*> graph;
    std::map<TypeNameVer, StructVer*> graphInv;
    void check_exist(const TypeNameVer& nameVer, bool suppos) const
    {
        bool activity = (graph.find(nameVer) != graph.end());
        if(activity != suppos)
        {
            std::cerr << "Error" << std::endl;
            exit(0);
        }
    }
};

#endif // GRAPH_H_INCLUDED
