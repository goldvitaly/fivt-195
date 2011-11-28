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

#include "digit.cpp"

template <class T1, class T2, class Comp1 = std::less<T1>, class Comp2 = std::less<T2> > 
class make_pair_comparator
{
public:
	bool operator () (std::pair<T1,T2> a, std::pair<T1,T2> b)
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
	bool operator ()(T a, T b)
	{
		return Comp()(b,a);
	}
};


class file_manager
{
	std::vector<std::string> created_files;	
	static const int length = 10;
	static const int alphabet_size = 26;
	int current_file_mask;
	std::string convert_to_string(int a) 
	{
		std::string result(length,'a');
		for (int i = 0; i < length; i ++)
		{
			result[i] = 'a' + (a % alphabet_size);
			a /= alphabet_size;
		}
		return result;
	}
	bool exists(std::string s)
	{
		return access(s.c_str(),0) != -1;
	}
	public:
		file_manager()
		{
			current_file_mask = 0;
		};
		~file_manager()
		{
			for (int i = 0; i < created_files.size(); i ++)
				remove(created_files[i].c_str());
		}
		std::string get_unused_file_name()
		{
			while (exists(convert_to_string(current_file_mask))) current_file_mask ++;
			std::string file_name = convert_to_string(current_file_mask ++);
			created_files.push_back(file_name);
			return file_name;
		}
};

template <class T, class Comp = std::less<T> >
class sorter
{
public:
	typedef T value_type;
	typedef Comp comparator;
	template <class It>
	void operator() (It l, It r)
	{
		sort(l,r,Comp());
	}
};

template <class PathName, class Sort>
void out_sort(PathName path_to_file, Sort sort, int block_size = 1000000)
{
	typedef typename Sort::value_type value_type;
	typedef typename Sort::comparator comp;
	value_type a;
	file_manager manager;
	std::string path = (std::string) path_to_file;
	std::ifstream fin(path.c_str(), std::ifstream::in);
	std::vector <value_type> v;
	std::vector<std::ifstream*> files;
	bool finish_flag = 1; 
	while (finish_flag)
	{
		finish_flag = (fin >> a);
		v.push_back(a);
		if (v.size() == block_size || !finish_flag)
		{
			sort(v.begin(),v.end());
			std::string file_name = manager.get_unused_file_name();
			std::cerr << file_name << std::endl;
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
	std::priority_queue <
		std::pair < value_type, std::ifstream*>, 
		std::vector < std::pair < value_type, std::ifstream* > >,
		reverse_comparator < make_pair_comparator<value_type,std::ifstream*, comp> > 
	> s; 
//	std::set< std::pair <value_type,std::ifstream*> , make_pair_comparator<value_type, std::ifstream*, comp> > s;
	for (int i = 0; i < files.size(); i ++)
	{
		value_type tmp;
		(*files[i]) >> tmp;
		s.push(std::make_pair(tmp,files[i]));
	}
	std::ofstream fout(path.c_str(),std::ofstream::out);
	int number_of_elements = 0;
	while (s.size() > 0)
	{
		std::pair<int,std::ifstream*> w = s.top(); s.pop();
		fout << w.first << std::endl; 
		if ((*(w.second)) >> w.first)
			s.push(w);
		else
			delete w.second;
	}
	fout << std::endl;
	fout.close();
};

template <class PathName, class value_type>
bool is_sorted(PathName p, value_type a)
{
	std::string path = std::string(p);
	std::ifstream fin(path.c_str(), std::ifstream::in);
	int count = 0;
	value_type prev = a;
	while (fin >> a)
	{
		if ((a < prev) && count) return false;
		prev = a;
		count ++;
	}
	return true;
}


int main()
{
	std::ios_base::sync_with_stdio(false);
	double start = clock() / (double) CLOCKS_PER_SEC;
//	out_sort("data.in", sorter<int>());
	out_sort("data.in", digit_sorter<int,int_extractor>(int_extractor(8)));
	fprintf(stderr, "out_sort works %.3lf seconds\n", clock() / (double) CLOCKS_PER_SEC - start);
	assert(is_sorted("data.in", int()));
	return 0;
};
