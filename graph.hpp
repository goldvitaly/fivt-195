#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <memory>
#include <utility>
#include "incident.hpp"

/**
   * Что нужно в дейкстре?
   * 1. Тип веса ребра. (EdgeWeight)
   * 2. Тип веса пути. (PathWeight)
   * 3. Класс для сложения типа пути и типа ребра. (PathWeightComp)
   * 4. Класс для сравнения двух путей (кто из них "короче"). ()
   * Схема дейкстры.
   * 1. Инициализируем путь начальной вершины (класса путь должен иметь метод makeStartPathWeight).
   * 2. Инициализируем остальные вершины (аналогично должен быть сетод makeUndefinedPathWeight).
   * 2.1. По умолчанию класс путь создает то же, что получается в результате выполнения makeUndefinedPathWeight.
   * 3. Создаем множество текущих(незакрытых вершин)
   * 4. В цикле ищем кратчайшую (сравнение при помощи класса сравнения путей) из незакрытых PathWeight.
   * 5. Релаксируем, используя класс суммы пути и ребра и сравнения путей.
   * Итог:
   * 1. Шаблоны класса Graph: <EdgeWeight>
   * 2. Шаблоны класса Inciden: <EdgeWeigth(получена от класса Graph)>
   * 3. Шаблоны класса DijkstraAlgorithm: <PathWeight, PathWeightComp, RelaxWeight>
**/

template<typename EdgeWeight> 
class Graph
{
public:
    Graph () {}
    template <typename IncidentClass> size_t AddVertex ()
    {
        _Inc.push_back(std::unique_ptr<IncidentClass>(new IncidentClass()));
        return _Inc.size() - 1;
    }
    void RemoveVertex (size_t Vertex)
    {
        if (Vertex >= _Inc.size()) return;

        for (size_t v = 0; v < _Inc.size(); ++v)
        {
            (*_Inc[v]).RemoveIncident(Vertex);
        }

        (*_Inc[Vertex]).clear();
    }
    void AddIncident (size_t VertexA, size_t VertexB, EdgeWeight edge)
    {
        _Inc[VertexA]->AddIncident(std::make_pair(VertexB, edge));
    }
    bool CheckIncident (size_t VertexA, size_t VertexB) const
    {
        if (VertexA >= _Inc.size()) return false;
        return _Inc[VertexA]->CheckIncident(VertexB);
    }
    size_t VertexNum () const
    {
        return _Inc.size();
    }
    size_t IncidentNum () const
    {
        size_t num = 0;
        for (size_t i = 0; i < _Inc.size(); ++i)
            num += _Inc[i]->IncidentNum();
        return num;
    }
    const Incident<EdgeWeight>& GetIncident (size_t Vertex) const
    {
        return *_Inc[Vertex];
    }
private:
    std::vector <std::unique_ptr<Incident<EdgeWeight>>> _Inc;
};

#endif /* GRAPH_HPP */
