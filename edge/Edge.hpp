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

template<typename FlowType>
class WithFlow
{
 public:
  FlowType capacity, flow;
  WithFlow(): capacity(0), flow(0) {}
  WithFlow(FlowType capacity, FlowType flow): capacity(capacity), flow(flow) {}

};

template<typename Weight>
class WeightedEdge: public BasicEdge, public Weighted<Weight>
{
 public:
  WeightedEdge(size_t source, size_t destination, Weight weight): BasicEdge(source, destination),
                                                                  Weighted<Weight>(weight) {}
};

template<typename FlowType>
class FlowEdge: public BasicEdge, public WithFlow<FlowType>, public Indexed
{
 public:
  FlowEdge(size_t source, size_t destination, 
           FlowType capacity, FlowType flow, 
           size_t index, size_t backEdgeIndex): BasicEdge(source, destination),
                                                WithFlow<FlowType>(capacity, flow),
                                                Indexed(index),
                                                backEdgeIndex(backEdgeIndex) {}
  size_t backEdgeIndex;                                                                                  
  FlowType getResidualCapacity() const
  {
    return this->capacity - this->flow;
  }
};

#endif /* EDGE_HPP */
