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
    void add_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.insert(nameVer);
    }
    void delete_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.erase(nameVer);
    }
    std::vector<TypeNameVer>* list_neighbour() const
    {
        //set<TypeNameVer>::iterator it;
        std::vector<TypeNameVer>* Vec = new std::vector<TypeNameVer>;
        Vec->resize(neighbours.size());
        std::copy(neighbours.begin(), neighbours.end(), Vec->begin());
        return Vec;
    }
    size_t degree() const
    {
        return neighbours.size();
    }
private:
    std::set<TypeNameVer> neighbours;
};


template<class TypeNameVer, class StructVer>
class Graph {
public:
    void add_vertex(const TypeNameVer& nameVer, const StructVer& structVer = StructVer())
    {
        check_exist(nameVer, false);
        graph.insert(std::make_pair(nameVer, structVer));
    }
    void delete_vertex(const TypeNameVer& nameVer)
    {
        check_exist(nameVer, true);
        graph.erase(nameVer);
    }
    void add_edge(const TypeNameVer& outVer, const TypeNameVer& inVer)
    {
        check_exist(outVer, true);
        check_exist(inVer, true);
        graph[outVer].add_neighbour(inVer);
    }
    void delete_edge(const TypeNameVer& outVer, const TypeNameVer& inVer)
    {
        check_exist(outVer, true);
        check_exist(inVer, true);
        graph[outVer].delete_neighbour(inVer);
    }
    std::vector<TypeNameVer>* list_neighbour(const TypeNameVer& nameVer)
    {
        check_exist(nameVer, true);
        return graph[nameVer].list_neighbour();
    }
    size_t size() const
    {
        return graph.size();
    }
private:
    std::map <TypeNameVer, StructVer> graph;
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
