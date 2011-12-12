#include <iostream>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <string>
#include <set>
#include <vector>
#include <cassert>
#include <ctime>
#include <queue>

#include "digit.h"

template <class T1, class T2, class Comp1 = std::less<T1>, class Comp2 = std::less<T2> > 
class make_pair_comparator
{
public:
	bool operator () (const std::pair<T1,T2>& a, const std::pair<T1,T2>& b)
	{
		if (!(Comp1()(a.first,b.first)) && !(Comp1()(b.first,a.first)))
			return Comp2()(a.second,b.second);
		else
			return Comp1()(a.first,b.first);
	}
};

template <class Comp>
class reverse_comparator
{
public:
	template <class T>
	bool operator ()(const T& a, const T& b)
	{
		return Comp()(b,a);
	}
};


class file_manager
{
	// Generates file names - constant size string of small latin letters
	// All files are deleted when file_manager is removed
	std::vector<std::string> created_files;	
	static const int length = 10;
	static const int alphabet_size = 26;
	int current_file_number;
	std::string get_file_name(int file_number) 
	{
		std::string result(length,'a');
		for (int i = 0; i < length; i ++)
		{
			result[i] = 'a' + (file_number % alphabet_size);
			file_number /= alphabet_size;
		}
		return result;
	}
	bool exists(std::string file) const
	{
		return access(file.c_str(),0) != -1;
	}
	public:
		file_manager()
		{
			current_file_number = 0;
		};
		~file_manager()
		{
			for (int i = 0; i < created_files.size(); i ++)
				remove(created_files[i].c_str());
		}
		std::string get_unused_file_name()
		{
			std::string file_name;
			do 
			{
				file_name = get_file_name(current_file_number);
				current_file_number ++;
			}
			while (exists(file_name));
			created_files.push_back(file_name);
			return file_name;
		}
};

template <class T, class Comparator = std::less<T> >
class sorter
{
public:
	Comparator comp;
	explicit sorter(Comparator c = Comparator()): comp(c)
	{

	}
	typedef T value_type;
	typedef Comparator comparator;
	template <class It>
	void operator() (It l, It r)
	{
		sort(l,r,comp);
	}
};

template <class Sort>
void out_sort(std::string path, Sort sort, int block_size = 1000000)
{
	typedef typename Sort::value_type value_type;
	typedef typename Sort::comparator comp;
	value_type a;
	file_manager manager;
	std::ifstream fin(path.c_str(), std::ifstream::in);
	std::vector <value_type> v;
	v.reserve(block_size);
	std::vector<std::ifstream*> files;
	bool finish_flag = 1; 
	while (finish_flag)
	{
		finish_flag = (fin >> a);
		if (finish_flag)
			v.push_back(a);
		if (v.size() == block_size || (!finish_flag && v.size() != 0))
		{
			sort(v.begin(),v.end());
			std::string file_name = manager.get_unused_file_name();
			std::cerr << "File " << file_name << " opened for writing" << std::endl;
			std::ofstream fout(file_name.c_str(), std::ofstream::out);
			for (int i = 0; i < v.size(); i ++)
			{
				fout << v[i];
				if (i != v.size() - 1) 
					fout << " ";
			}
			fout << std::endl;
			fout.close();
			v.clear();
			files.push_back(new std::ifstream(file_name.c_str(), std::ifstream::in));
		}
	};
	typedef std::pair < value_type, std::ifstream* > FileWithFirstValue;
	std::priority_queue <
		FileWithFirstValue,
		std::vector < FileWithFirstValue >, 
		reverse_comparator < make_pair_comparator<value_type,std::ifstream*, comp> > 
	> s; 
	for (int i = 0; i < files.size(); i ++)
	{
		value_type value;
		(*files[i]) >> value;
		s.push(std::make_pair(value,files[i]));
	}
	std::ofstream fout(path.c_str(),std::ofstream::out);
	size_t number_of_elements = 0;
	while (s.size() > 0)
	{
		FileWithFirstValue w = s.top(); s.pop();
		fout << w.first << std::endl; 
		if ((*(w.second)) >> w.first)
			s.push(w);
		else
			delete w.second;
	}
	fout.close();
};

template <class value_type, class Comparator = std::less <value_type> >
bool is_sorted(std::string path, Comparator comparator = Comparator())
{
	std::ifstream fin(path.c_str(), std::ifstream::in);
	int count = 0;
	value_type prev, cur;
	while (fin >> cur)
	{
		if ((cur < prev) && count) return false;
		prev = cur;
		count ++;
	}
	return true;
}


int main()
{
	std::ios_base::sync_with_stdio(false);
	double start = clock() / (double) CLOCKS_PER_SEC;
//	out_sort("data.in", sorter<int>());
	out_sort("data.in", integral_digit_sorter<int>(integral_extractor<int>()));
	fprintf(stderr, "out_sort works %.3lf seconds\n", clock() / (double) CLOCKS_PER_SEC - start);
	assert(is_sorted<int>("data.in"));
	return 0;
};
