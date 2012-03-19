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
#include <set>
#include <algorithm>
#include <memory>

#define print(x) std::cout << x << std::endl;

class IncidenceType
{
 public:
  class Iterator;
  
  virtual void     addEdge(int destination) = 0;
  virtual void     removeEdge(int destination) = 0;
  virtual void     clear() = 0;
  virtual bool     isConnectedTo(int destination) const = 0;
  virtual size_t   degree() const = 0;
  virtual Iterator begin() const = 0;
  virtual Iterator end()   const = 0;

  class BaseIterator
  {
   public:
    virtual bool operator ==(const BaseIterator& it) const = 0;
    virtual bool operator !=(const BaseIterator& it) const = 0;
    virtual size_t        operator *() const   = 0;
    virtual BaseIterator& operator ++()  = 0;
    virtual ~BaseIterator();
  }; // BaseIterator
  
  class Iterator
  {
   public:
    explicit Iterator(BaseIterator* base_iterator)
    {
      iterator_ = std::unique_ptr<BaseIterator>(base_iterator);
    }

    Iterator(IncidenceType::Iterator&& iterator)
    {
      iterator_ = std::move(iterator.iterator_);
    }

    size_t operator *() const
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

IncidenceType::BaseIterator::~BaseIterator() {}

class VertexIncidenceType
{
 public:
  explicit VertexIncidenceType(IncidenceType* incident_type)
  {
    incident_type_ = std::unique_ptr<IncidenceType> (incident_type);
  }

  explicit VertexIncidenceType(VertexIncidenceType&& vertex_incidence_type)
  {
    incident_type_ = std::move(vertex_incidence_type.incident_type_);
  }

  void addEdge(int destination)
  {
    if (destination < 0)
      throw std::out_of_range("Illegal vertex index");
    incident_type_->addEdge(destination);
  }

  void removeEdge(int destination)
  {
    if (destination < 0)
      throw std::out_of_range("Illegal vertex index");
    if (destination >= degree())
      return;
    incident_type_->removeEdge(destination);
  }

  void clear()
  {
    incident_type_->clear();
  }

  bool isConnectedTo(int destination) const
  {
    if (destination < 0)
      throw std::out_of_range("Illegal vertex index");
    if (destination >= degree())
      return 0;
    return incident_type_->isConnectedTo(destination);
  }

  size_t degree() const
  {
    return incident_type_->degree();
  }

  IncidenceType::Iterator begin() const
  {
    return incident_type_->begin();
  }

  IncidenceType::Iterator end() const
  {
    return incident_type_->end();
  }
  
 private:
  std::unique_ptr<IncidenceType> incident_type_;

}; // VertexIncidenceType

class SetIncidence : public IncidenceType
{
 public: 
  void addEdge(int destination)
  {
    incident_.insert(destination);
  }

  void removeEdge(int destination)
  {
    incident_.erase(destination);
  }

  void clear()
  {
    incident_.clear();
  }

  bool isConnectedTo(int destination) const
  {
    return (incident_.find(destination) != incident_.end());
  }

  size_t degree() const
  {
    return incident_.size();
  }

  Iterator begin() const
  {
    return Iterator(new SetBaseIterator(incident_.begin()));
  }

  Iterator end() const
  {
    return Iterator(new SetBaseIterator(incident_.end()));
  }

  class SetBaseIterator : public BaseIterator
  {
   public:
    explicit SetBaseIterator(std::set<size_t>::const_iterator it)
    {
      set_iterator = it;
    }

    size_t operator *() const
    {
      return *set_iterator;
    }

    SetBaseIterator& operator ++()
    {
      set_iterator++;
      return *this;
    }

    bool operator ==(const BaseIterator& it) const
    {
      const SetBaseIterator& s_it = dynamic_cast<const SetBaseIterator&> (it);
      return (set_iterator == s_it.set_iterator);
    }

    bool operator !=(const BaseIterator& it) const
    {
      return !(*this == it);
    }

    ~SetBaseIterator()
    {
    }

   private:
    std::set<size_t>::const_iterator set_iterator;

  }; // SetBaseIterator

 private:
  std::set<size_t> incident_;

}; // SetIncidence

class VectorIncidence : public IncidenceType
{
 public: 
  void addEdge(int destination)
  {
    incident_.push_back(destination);
  }

  void removeEdge(int destination)
  {
    std::vector<size_t>::iterator it = find(incident_.begin(), incident_.end(), destination);
    if (it != incident_.end())
      incident_.erase(it);
  }

  void clear()
  {
    incident_.clear();
  }

  bool isConnectedTo(int destination) const
  {
    return (find(incident_.begin(), incident_.end(), destination) != incident_.end());
  }

  size_t degree() const
  {
    return incident_.size();
  }

  Iterator begin() const
  {
    return Iterator(new VectorBaseIterator(incident_.begin()));
  }

  Iterator end() const
  {
    return Iterator(new VectorBaseIterator(incident_.end()));
  }
  
  class VectorBaseIterator : public BaseIterator
  {
   public:
    explicit VectorBaseIterator(std::vector<size_t>::const_iterator it)
    {
      vector_iterator = it;
    }

    size_t operator *() const
    {
      return *vector_iterator;
    }

    VectorBaseIterator& operator ++()
    {
      vector_iterator++;
      return *this;
    }

    bool operator ==(const BaseIterator& it) const
    {
      const VectorBaseIterator& v_it = dynamic_cast<const VectorBaseIterator&> (it);
      return (vector_iterator == v_it.vector_iterator);
    }

    bool operator !=(const BaseIterator& it) const
    {
      return !(*this == it);
    }

    ~VectorBaseIterator()
    {
    }

   private:
    std::vector<size_t>::const_iterator vector_iterator;
    
  }; // VectorBaseIterator

 private:
  std::vector<size_t> incident_;

}; // ListIncidence

class BitmaskIncidence : public IncidenceType
{
 public: 
  void addEdge(int destination)
  {
    if (incident_.size() < destination + 1)
      incident_.resize(destination + 1);
    incident_[destination] = 1;
  }
  
  void removeEdge(int destination)
  {
    incident_[destination] = 0;
  }

  void clear()
  {
    incident_.clear();
  }

  bool isConnectedTo(int destination) const
  {
    return incident_[destination];
  }

  size_t degree() const
  {
    return count(incident_.begin(), incident_.end(), 1);
  }

  Iterator begin() const
  {
    BitmaskBaseIterator* it = new BitmaskBaseIterator(-1, &incident_);
    ++(*it);
    return Iterator(it);
  }

  Iterator end() const
  {
    return Iterator(new BitmaskBaseIterator(incident_.size(), &incident_));
  }

  class BitmaskBaseIterator : public BaseIterator
  {
   public:
    explicit BitmaskBaseIterator(int index, const std::vector<bool>* incident)
    {
      iterator_ = index;
      incident_ = incident;
    }

    size_t operator *() const
    {
      return iterator_;
    }

    BitmaskBaseIterator& operator++()
    {
      iterator_++;
      if (iterator_ >= incident_->size())
        iterator_ = incident_->size();
      else
        while (iterator_ < incident_->size() && !(incident_->at(iterator_)))
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
    const std::vector<bool>* incident_;

  }; // BitmaskBaseIterator

 private:
  std::vector<bool> incident_;

}; // BitsIncidence

#endif /* INCIDENCETYPE_HPP */
