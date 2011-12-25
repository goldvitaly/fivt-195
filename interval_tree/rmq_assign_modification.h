#ifndef RMQ_ASSIGN_H_
#define RMQ_ASSIGN_H_

#include <stdexcept>

#include "interval_tree.h"
#include "functors.h"



template <class T, class Comparator = std::less<T> > 
class RMQAssignModification
{
  public:
	Comparator comparator;
	explicit RMQAssignModification(int size_, const T& initial_value = T(), Comparator comparator = Comparator()): 
		comparator(comparator), size_(size_),
		tree(size_, initial_value, functors::min<T,Comparator>(comparator), functors::assign<T>(), functors::assign<T>())
	{
	}
	void apply(int l, int r, const T& modification)
	{
		tree.apply(l, r, modification);
	}
	T request(int l, int r)
	{
		return tree.request(l, r);
	}
	int size() const { return size_; };
  private:
	int size_;
	IntervalTree <T, functors::min<T,Comparator>, T, functors::assign<T>, functors::assign<T> > tree;
};

#endif // RMQ_ASSIGN_H_
