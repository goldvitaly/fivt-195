/*
 * =====================================================================================
 *
 *       Filename:  IncidenceType.hpp
 *
 *    Description:  IncidenceType class description
 *
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */
#ifndef INCIDENCETYPE_HPP
#define INCIDENCETYPE_HPP 

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <memory>

#define print(x) std::cout << x << std::endl;

template<typename EdgeType>
class IncidenceType
{
 public:
  class Iterator;
  
  virtual void     addEdge(EdgeType edge) = 0;
  virtual void     removeEdge(EdgeType edge) = 0;
  virtual void     clear() = 0;
  virtual bool     hasEdge(EdgeType edge) const = 0;
  virtual size_t   degree() const = 0;
  virtual Iterator begin() const = 0;
  virtual Iterator end()   const = 0;
  virtual ~IncidenceType() {}

  class BaseIterator
  {
   public:
    virtual bool operator ==(const BaseIterator& it) const = 0;
    virtual bool operator !=(const BaseIterator& it) const = 0;
    virtual EdgeType      operator *() const   = 0;
    virtual BaseIterator& operator ++()  = 0;
    virtual ~BaseIterator() {};
  }; // BaseIterator
  
  class Iterator
  {
   public:
    explicit Iterator(BaseIterator* base_iterator)
    {
      iterator_ = std::unique_ptr<BaseIterator>(base_iterator);
    }

    Iterator(IncidenceType<EdgeType>::Iterator&& iterator)
    {
      iterator_ = std::move(iterator.iterator_);
    }

    EdgeType operator *() const
    {
      return **iterator_;
    }

    Iterator& operator ++()
    {
      ++(*iterator_);
      return *this;
    }
    
    bool operator ==(const Iterator& it) const
    {
      return *iterator_ == *it.iterator_;
    }

    bool operator !=(const Iterator& it) const
    {
      return *iterator_ != *it.iterator_;
    }

    ~Iterator()
    {
    }

   private:
    std::unique_ptr<BaseIterator> iterator_;

  }; // Iterator

 private:
  
}; // IncidenceType

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

template<typename EdgeType>
class VectorIncidence : public IncidenceType<EdgeType>
{
 public: 
  void addEdge(EdgeType edge)
  {
    incident_.push_back(edge);
  }

  void removeEdge(EdgeType edge)
  {
    typename std::vector<EdgeType>::iterator it = find(incident_.begin(), incident_.end(), edge);
    if (it != incident_.end())
      incident_.erase(it);
  }

  void clear()
  {
    incident_.clear();
  }

  bool hasEdge(EdgeType edge) const
  {
    return find(incident_.begin(), incident_.end(), edge) != incident_.end();
  }

  size_t degree() const
  {
    return incident_.size();
  }

  typename IncidenceType<EdgeType>::Iterator begin() const
  {
    return typename IncidenceType<EdgeType>::Iterator(new VectorBaseIterator(incident_.begin()));
  }

  typename IncidenceType<EdgeType>::Iterator end() const
  {
    return typename IncidenceType<EdgeType>::Iterator(new VectorBaseIterator(incident_.end()));
  }
  
  class VectorBaseIterator : public IncidenceType<EdgeType>::BaseIterator
  {
   public:
    explicit VectorBaseIterator(typename std::vector<EdgeType>::const_iterator it) : vector_iterator(it)
    {
    }

    EdgeType operator *() const
    {
      return *vector_iterator;
    }

    VectorBaseIterator& operator ++()
    {
      vector_iterator++;
      return *this;
    }

    bool operator ==(const typename IncidenceType<EdgeType>::BaseIterator& it) const
    {
      const VectorBaseIterator& v_it = dynamic_cast<const VectorBaseIterator&> (it);
      return (vector_iterator == v_it.vector_iterator);
    }

    bool operator !=(const typename IncidenceType<EdgeType>::BaseIterator& it) const
    {
      return !(*this == it);
    }

    ~VectorBaseIterator()
    {
    }

   private:
    typename std::vector<EdgeType>::const_iterator vector_iterator;
    
  }; // VectorBaseIterator

 private:
  std::vector<EdgeType> incident_;

}; // ListIncidence

template<typename EdgeType>
class SetIncidence : public IncidenceType<EdgeType>
{
 public: 
  void addEdge(EdgeType edge)
  {
    incident_.insert(edge);
  }

  void removeEdge(EdgeType edge)
  {
    incident_.erase(edge);
  }

  void clear()
  {
    incident_.clear();
  }

  bool hasEdge(EdgeType edge) const
  {
    return (incident_.find(edge) != incident_.end());
  }

  size_t degree() const
  {
    return incident_.size();
  }

  typename IncidenceType<EdgeType>::Iterator begin() const
  {
    return typename IncidenceType<EdgeType>::Iterator(new SetBaseIterator(incident_.begin()));
  }

  typename IncidenceType<EdgeType>::Iterator end() const
  {
    return typename IncidenceType<EdgeType>::Iterator(new SetBaseIterator(incident_.end()));
  }

  class SetBaseIterator : public IncidenceType<EdgeType>::BaseIterator
  {
   public:
    explicit SetBaseIterator(typename std::unordered_set<EdgeType>::const_iterator it) : set_iterator(it)
    {
    }

    EdgeType operator *() const
    {
      return *set_iterator;
    }

    SetBaseIterator& operator ++()
    {
      set_iterator++;
      return *this;
    }

    bool operator ==(const typename IncidenceType<EdgeType>::BaseIterator& it) const
    {
      const SetBaseIterator& s_it = dynamic_cast<const SetBaseIterator&> (it);
      return (set_iterator == s_it.set_iterator);
    }

    bool operator !=(const typename IncidenceType<EdgeType>::BaseIterator& it) const
    {
      return !(*this == it);
    }

    ~SetBaseIterator()
    {
    }

   private:
    typename std::unordered_set<EdgeType>::const_iterator set_iterator;

  }; // SetBaseIterator

 private:
  std::unordered_set<EdgeType> incident_;

}; // SetIncidence

/*
class BitmaskIncidence : public IncidenceType<EdgeType>
{
 public: 
  void addEdge(size_t destination)
  {
    if (incident_.size() < destination + 1)
      incident_.resize(destination + 1);
    incident_[destination] = 1;
  }
  
  void removeEdge(size_t destination)
  {
    incident_[destination] = 0;
  }

  void clear()
  {
    incident_.clear();
  }

  bool isConnectedTo(size_t destination) const
  {
    return incident_[destination];
  }

  size_t degree() const
  {
    return count(incident_.begin(), incident_.end(), 1);
  }

  Iterator begin() const
  {
    BitmaskBaseIterator* it = new BitmaskBaseIterator(-1, incident_);
    ++(*it);
    return Iterator(it);
  }

  Iterator end() const
  {
    return Iterator(new BitmaskBaseIterator(incident_.size(), incident_));
  }

  class BitmaskBaseIterator : public BaseIterator
  {
   public:
    explicit BitmaskBaseIterator(int index, const std::vector<bool>& incident) : incident_(incident), 
                                                                                 iterator_(index)
    {
    }

    size_t operator *() const
    {
      return iterator_;
    }

    BitmaskBaseIterator& operator++()
    {
      iterator_++;
      if (iterator_ >= incident_.size())
        iterator_ = incident_.size();
      else
        while (iterator_ < incident_.size() && !(incident_.at(iterator_)))
          iterator_++;
      return *this;
    }

    bool operator ==(const BaseIterator& it) const
    {
      const BitmaskBaseIterator& b_it = dynamic_cast<const BitmaskBaseIterator&> (it);
      return (iterator_ == b_it.iterator_);
    }

    bool operator !=(const BaseIterator& it) const
    {
      return !(*this == it);
    }

    ~BitmaskBaseIterator()
    {
    }

   private:
    int iterator_;
    const std::vector<bool>& incident_;

  }; // BitmaskBaseIterator

 private:
  std::vector<bool> incident_;

}; // BitsIncidence
*/

#endif /* IncidenceType<EdgeType>_HPP */
