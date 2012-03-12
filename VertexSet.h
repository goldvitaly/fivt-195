#ifndef VERTEXSET_H_INCLUDED
#define VERTEXSET_H_INCLUDED

#include "graph.h"

template<class TypeNameVer>
class VertexSet : public Vertex <TypeNameVer>
{
public:
    void add_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.insert(nameVer);
    }
    void delete_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.erase(nameVer);
    }
    bool exist_neighbour(const TypeNameVer& nameVer)
    {
        return (neighbours.find(nameVer) != neighbours.end());
    }
    std::vector<TypeNameVer> list_neighbour() const
    {
        std::vector<TypeNameVer> Vec;
        Vec.resize(neighbours.size());
        std::copy(neighbours.begin(), neighbours.end(), Vec.begin());
        return Vec;
    }
    void for_each_neighbour(UnaryFunc<TypeNameVer>& unaryFunc)
    {
        typename std::set<TypeNameVer>::iterator it_begin = neighbours.begin();
        typename std::set<TypeNameVer>::iterator it_end = neighbours.end();
        while(it_begin != it_end)
        {
            unaryFunc(*(it_begin));
            it_begin++;
        }
    }
    size_t degree() const
    {
        return neighbours.size();
    }
    ~VertexSet<TypeNameVer>()
    {
        neighbours.clear();
    }
private:
    std::set<TypeNameVer> neighbours;
};


#endif // VERTEXSET_H_INCLUDED
