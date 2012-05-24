/*
 * =====================================================================================
 *
 *       Filename:  Utils.hpp
 *
 *    Description:  Lib with some utils
 *
 *        Version:  1.0
 *        Created:  03/18/2012 08:41:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */
#ifndef UTILS_HPP
#define UTILS_HPP 

#include <sstream>
#include <time.h>

template <class T>
inline std::string toString (const T& t)
{
  std::stringstream ss;
  ss << t;
  return ss.str();
}

class Timer
{
 public:

  Timer(std::string name)
  {
    name_ = name;
  }

  void start()
  {
    start_time_ = clock();
  }

  double getTime()
  {
    return (clock() - start_time_) / double(CLOCKS_PER_SEC);
  }

  void printTime()
  {
    printf("%s: %.3lf sec\n", name_.c_str(), getTime());
  }
  
 private:
  clock_t start_time_;
  std::string name_;

}; // Timer

#endif /* UTILS_HPP */
