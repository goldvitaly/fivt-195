#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

class ICB
{
public:
	virtual void operator() (int v)= 0;
};

class ICBprint : public ICB
{
public:
	void operator() (int v)
	{
		cout << v << endl;
	}
};

class pVertex
{
public:
	virtual void addNeighbour (const int v) = 0;
	virtual void delNeighbour (const int v) = 0;
	virtual const bool isConnect(const int v) = 0;
	virtual const size_t degree() = 0;
	virtual void clear() = 0;
	virtual void incedents (ICB& cb) = 0;
};

class Vvectorbool : public pVertex
{ 
private:
	vector<bool> adj;
	int deg;
public:
	Vvectorbool ()
	{
		deg = 0;
	}
	void addNeighbour (const int v)
	{
		if (v >= adj.size())
			adj.resize(v + 1);
		if (adj[v] == false)
			adj[v] = true, deg++;
	}
	void delNeighbour (const int v)
	{
		if (v >= adj.size())
			return;
		if (adj[v] == true)
			deg--, adj[v] = false;		
	}
	const bool isConnect(const int v)
	{
		if (v >= adj.size())
			return false;
		return adj[v];
	}
	const size_t degree()
	{
		return deg;
	}
	void clear()
	{
		adj.clear();
		deg = 0;
	}
	void incedents(ICB& cb)
	{
		for (int i = 0; i < adj.size(); i++)
			if (adj[i])
				cb(i);
	}
};

class Vvectorint : public pVertex
{ 
private:
	vector<int> adj;
	int deg;
public:
	Vvectorint ()
	{
		deg = 0;
	}
	void addNeighbour (const int v)
	{
		adj[v]++, deg++;
	}
	void delNeighbour (const int v)
	{
		if (v >= adj.size() || adj[v] == 0)
		{
			cerr << "Deleting error" << endl;
			return;
		}
		if (adj[v] > 0)
			deg--, adj[v]--;		
	}
	const bool isConnect(const int v)
	{
		if (v >= adj.size())
			return false;
		return (adj[v] > 0);
	}
	const size_t degree()
	{
		return deg;
	}
	void clear()
	{
		adj.clear();
		deg = 0;
	}
	void incedents(ICB& cb)
	{
		for (int i = 0; i < adj.size(); i++)
			if (adj[i])
				cb(i);
	}
};

class Vset : public pVertex
{
private:
	set<int> adj;
public:
	void addNeighbour (const int v)
	{
		adj.insert(v);
	}
	void delNeighbour (const int v)
	{
		set<int>:: iterator it;
		it = adj.find(v);
		if (it == adj.end())
			cerr << "Deleting error" << endl;	
		adj.erase(it);
	}
	const bool isConnect(const int v)
	{
		return (adj.find(v) != adj.end());
	}
	const size_t degree()
	{
		return adj.size();
	}
	void clear()
	{
		adj.clear();
	}
	void incedents(ICB& cb)
	{
		for (auto it = adj.begin(); it != adj.end(); it++)
			cb(*it);
	}
};

class Vmap : public pVertex
{
private:
	map<int, int> adj;
public:
	void addNeighbour (const int v)
	{
		adj[v]++;
	}
	void delNeighbour (const int v)
	{
		map<int, int>:: iterator it; // maybe static?
		it = adj.find(v);
		if (it == adj.end())
		{
			cerr << "Deleting error" << endl;
			return;
		}
		if ((*it).second == 1)
			adj.erase(it);
		else
			(*it).second--;
	}
	const bool isConnect(const int v)
	{
		return (adj.find(v) != adj.end());
	}
	const size_t degree()
	{
		return adj.size();
	}
	void clear()
	{
		adj.clear();
	}
	void incedents(ICB& cb)
	{
		for (auto it = adj.begin(); it != adj.end(); it++)
			cb((*it).first);
	}
};

class Vlist : public pVertex
{
private:
	vector<int> adj;
public:
	void addNeighbour (const int v)
	{
		adj.push_back(v);
	}
	void delNeighbour (const int v)
	{
		for (int pos = 0; pos < adj.size(); pos++)
			if (adj[pos] == v)
			{
				swap(adj[pos], adj[adj.size() - 1]);
				adj.pop_back();
				break;
			}	 
	}
	const bool isConnect(const int v) 
	{
		for (auto it = adj.begin(); it != adj.end(); it++)
			if ((*it) == v)
				return true;
		return false;
	}
	const size_t degree()
	{
		return adj.size();
	}
	void clear()
	{
		adj.clear();
	}
	void incedents(ICB& cb)
	{
		for (auto it = adj.begin(); it != adj.end(); it++)
			cb(*it);
	}
};


class Vertex
{
private:
	pVertex * pV;
public:
	Vertex (string type = "int") 
	{
		if (type == "bool")
		{
			pV = new Vvectorbool;
		}
		else
		if (type == "int")
		{
			pV = new Vvectorint;
		}	
		else
		if (type == "set")
		{
			pV = new Vset;
		}			
		else
		if (type == "map")
		{
			pV = new Vmap;
		}
		else
		if (type == "list")
		{
			pV = new Vlist;
		}	
	}; 
	~Vertex()
	{
		delete pV;
	};
	void addNeighbour (const int v)
	{
		pV->addNeighbour(v);
	};
	void delNeighbour (const int v)
	{
		pV->delNeighbour(v);
	};
	const bool isConnect(const int v)
	{
		return pV->isConnect(v);
	};
	const size_t degree()
	{
		return pV->degree();
	};
	void clear()
	{
		pV->clear();
	};
	void incedents (ICB& cb)
	{
		pV->incedents(cb);
	};
};