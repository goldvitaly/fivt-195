#ifndef INTERVAL_H_
#define INTERVAL_H_

#include <algorithm>  // for std::max
#include <cassert>    // for assert
#include <iostream>

class Interval
{
  public:
	  Interval left_half() const
	  {
		  return Interval(left_bound, (left_bound + right_bound) / 2);
	  }
	  Interval right_half() const
	  {
		  return Interval( (left_bound + right_bound) / 2, right_bound);
	  }
	  Interval()
	  {
		  left_bound = 0;
		  right_bound = 0;
	  }
	  Interval(int left, int right)
	  {
		  left_bound = left;
		  right_bound = right;
		  assert(left_bound < right_bound);
	  }
	  bool contains(const Interval& interval) const 
	  {
		  return (left_bound <= interval.left_bound && interval.right_bound <= right_bound);
	  }
	  bool intersects(const Interval& interval) const 
	  {
		  return !(left_bound >= interval.right_bound || right_bound <= interval.left_bound);
	  }
	  size_t size() const
	  {
		  return std::max(0, right_bound - left_bound);
	  }
	  int left_bound, right_bound;
	  static Interval intersect(const Interval& first, const Interval& second) 
	  {
		  return Interval( std::max(first.left_bound, second.left_bound), std::min(first.right_bound, first.right_bound)); 
	  }
};

std::ostream& operator << (std::ostream& stream, const Interval& interval)
{
	stream << "(" << interval.left_bound << ", " << interval.right_bound << ")";
	return stream;
}

#endif // INTERVAL_H_
