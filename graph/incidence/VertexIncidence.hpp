/*
 * =====================================================================================
 *
 *       Filename:  VertexIncidence.hpp
 *
 *    Description:  VertexIncidence class is a wrapper for IncidenceType
 *
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */
#ifndef VERTEXINCIDENCE_HPP
#define VERTEXINCIDENCE_HPP 

#include <cstdlib>
#include <cstdio>
#include <memory>

#include "incidence_impl/VectorIncidence.hpp"
#include "incidence_impl/SetIncidence.hpp"
#include "incidence_impl/BitmaskIncidence.hpp"

template<typename EdgeType>
class VertexIncidenceType
{
 public:
  explicit VertexIncidenceType(IncidenceType<EdgeType>* incident_type)
  {
    incident_type_ = std::unique_ptr< IncidenceType<EdgeType> > (incident_type);
  }

  explicit VertexIncidenceType(VertexIncidenceType<EdgeType>&& vertex_incidence_type)
  {
    incident_type_ = std::move(vertex_incidence_type.incident_type_);
  }

  void addEdge(EdgeType edge)
  {
    incident_type_->addEdge(edge);
  }

  void removeEdge(EdgeType edge)
  {
    incident_type_->removeEdge(edge);
  }

  void clear()
  {
    incident_type_->clear();
  }

  bool hasEdge(EdgeType edge) const
  {
    return incident_type_->hasEdge(edge);
  }

  size_t degree() const
  {
    return incident_type_->degree();
  }

  typename IncidenceType<EdgeType>::Iterator begin() const
  {
    return incident_type_->begin();
  }

  typename IncidenceType<EdgeType>::Iterator end() const
  {
    return incident_type_->end();
  }

  std::string getType() const
  {
    return typeid(*incident_type_).name();
  }
  
 private:
  std::unique_ptr< IncidenceType<EdgeType> > incident_type_;

}; // VertexIncidenceType<EdgeType>

#endif /* VERTEXINCIDENCE_HPP */
