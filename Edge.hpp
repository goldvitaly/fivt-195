/*
 * =====================================================================================
 *
 *       Filename:  Edge.hpp
 *
 *    Description:  Edge types description
 *
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */
#ifndef EDGE_HPP
#define EDGE_HPP 

class BasicEdge
{
 public:
  size_t source, destination;

  BasicEdge(size_t source, size_t destination): source(source), destination(destination) {}

  bool operator==(const BasicEdge& edge) const
  {
    return ((source == edge.source) && (destination == edge.destination));
  }

  bool operator!=(const BasicEdge& edge) const
  {
    return !(*this == edge);
  }
};

namespace std
{

template<>
struct hash<BasicEdge>
{
  size_t operator()(const BasicEdge& edge) const
  {
    return hash<size_t>()(edge.source) ^ hash<size_t>()(edge.destination);
  }
};

} // namespace std

template<typename Weight>
struct Weighted
{
 public:
  Weight weight;
  Weighted(Weight weight): weight(weight) {}
};

class Indexed
{
 public:
  size_t index;
  Indexed(): index(0) {}
  Indexed(size_t index): index(index) {}
};

template<typename Weight>
class WeightedEdge: public BasicEdge, public Weighted<Weight>
{
 public:
  WeightedEdge(size_t source, size_t destination, Weight weight): BasicEdge(source, destination),
                                                                  Weighted<Weight>(weight) {}
};

#endif /* EDGE_HPP */
