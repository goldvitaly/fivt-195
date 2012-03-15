/*
 * =====================================================================================
 *
 *       Filename:  IncidentType.hpp
 *
 *    Description:  IncidentType class description
 *
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */
#ifndef INCIDENTTYPE_HPP
#define INCIDENTTYPE_HPP 

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <set>
#include <algorithm>
#include <memory>

class IncidentType
{
 public:
  class Iterator;
  
  virtual void     addEdge(int destination)    = 0;
  virtual void     removeEdge(int destination) = 0;
  virtual bool     isConnectedTo(int destination) const = 0;
  virtual size_t   edgeCount() const = 0;
  virtual Iterator begin() const = 0;
  virtual Iterator end()   const = 0;

  class BaseIterator
  {
   public:
    virtual bool operator ==(BaseIterator& it) const = 0;
    virtual bool operator !=(BaseIterator& it) const = 0;
    virtual size_t        operator *() const   = 0;
    virtual BaseIterator& operator ++()  = 0;
    virtual ~BaseIterator();
  };
  
  class Iterator
  {
   public:
    explicit Iterator(BaseIterator* base_iterator)
    {
      iterator_ = std::unique_ptr<BaseIterator>(base_iterator);
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
    bool operator ==(Iterator it) const
    {
      return *iterator_ == *it.iterator_;
    }
    bool operator !=(Iterator it) const
    {
      return *iterator_ != *it.iterator_;
    }
    ~Iterator()
    {
    }

   private:
    std::unique_ptr<BaseIterator> iterator_;

  };

 private:
  
}; // IncidentType

IncidentType::BaseIterator::~BaseIterator() {}

class SetIncident : public IncidentType
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
  bool isConnectedTo(int destination) const
  {
    return (incident_.find(destination) != incident_.end());
  }
  size_t edgeCount() const
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
    explicit SetBaseIterator(std::set<int>::const_iterator it)
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
    bool operator ==(BaseIterator& it) const
    {
      SetBaseIterator& s_it = dynamic_cast<SetBaseIterator&> (it);
      return (set_iterator == s_it.set_iterator);
    }
    bool operator !=(BaseIterator& it) const
    {
      return !(*this == it);
    }
    ~SetBaseIterator()
    {
    }

   private:
    std::set<int>::const_iterator set_iterator;

  };

 private:
  std::set<int> incident_;
}; // SetIncidence

class VectorIncident : public IncidentType
{
 public: 
  void addEdge(int destination)
  {
    incident_.push_back(destination);
  }
  void removeEdge(int destination)
  {
    std::vector<int>::iterator it = find(incident_.begin(), incident_.end(), destination);
    if (it != incident_.end())
      incident_.erase(it);
  }
  bool isConnectedTo(int destination) const
  {
    return (find(incident_.begin(), incident_.end(), destination) != incident_.end());
  }
  size_t edgeCount() const
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
    explicit VectorBaseIterator(std::vector<int>::const_iterator it)
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
    bool operator ==(BaseIterator& it) const
    {
      VectorBaseIterator& v_it = dynamic_cast<VectorBaseIterator&> (it);
      return (vector_iterator == v_it.vector_iterator);
    }
    bool operator !=(BaseIterator& it) const
    {
      return !(*this == it);
    }
    ~VectorBaseIterator()
    {
    }

   private:
    std::vector<int>::const_iterator vector_iterator;
    
  };

 private:
  std::vector<int> incident_;
}; // ListIncidence

class BitmaskIncident : public IncidentType
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
    if (incident_.size() > destination)
      incident_[destination] = 0;
  }
  bool isConnectedTo(int destination) const
  {
    if (incident_.size() < destination + 1)
      return 0;
    return incident_[destination];
  }
  size_t edgeCount() const
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
    bool operator ==(BaseIterator& it) const
    {
      BitmaskBaseIterator& b_it = dynamic_cast<BitmaskBaseIterator&> (it);
      return (iterator_ == b_it.iterator_);
    }
    bool operator !=(BaseIterator& it) const
    {
      return !(*this == it);
    }
    ~BitmaskBaseIterator()
    {
    }

   private:
    int iterator_;
    const std::vector<bool>* incident_;

  };

 private:
  std::vector<bool> incident_;
}; // BitsIncidence

#endif /* INCIDENTTYPE_HPP */
