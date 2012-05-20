#ifndef USEFUL_H
#define USEFUL_H

#include <iostream>
#include <vector>

template <class T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& v)
{
	for (size_t i = 0; i < v.size(); i ++)
	{
		out << v[i];
		if (i + 1 != v.size())
			out << " ";
	}
	return out;
}

template <class T>
std::ostream& operator << (std::ostream& out, const std::set<T>& s)
{
	int cnt = 0;
	for (typename std::set<T>::iterator it = s.begin(); it != s.end(); it ++)
	{
		if (cnt > 0) out << " ";
		out << *it;		
		cnt ++;
	}
	return out;
};

#endif //USEFUL_H
