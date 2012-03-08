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
	virtual void operator() (size_t v)= 0;
};

class ICBprint : public ICB
{
public:
	void operator() (size_t v)
	{
		cout << v << endl;
	}
};

class pVertex
{
public:
	virtual void addNeighbour (const size_t v) = 0;
	virtual void delNeighbour (const size_t v) = 0;
	virtual bool isConnect(const size_t v) const = 0;
	virtual size_t degree() const = 0;
	virtual void clear() = 0;
	virtual void incedents (ICB& cb) = 0;
};

class Vvectorbool : public pVertex
{ 
private:
	vector<bool> adj;
	size_t deg;
public:
	Vvectorbool () : deg(0) {};
	void addNeighbour (const size_t v)
	{
		if (v >= adj.size())
			adj.resize(v + 1);
		if (adj[v] == false)
		{
			adj[v] = true;
			deg++;
		}
	}
	void delNeighbour (const size_t v)
	{
		if (v >= adj.size())
			return;
		if (adj[v] != false)
		{
			deg--;
			adj[v] = false;
		}		
	}
	bool isConnect(const size_t v) const
	{
		if (v >= adj.size())
			return false;
		return adj[v];
	}
	size_t degree() const 
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
		for (size_t i = 0; i < adj.size(); i++)
			if (adj[i])
				cb(i);
	}
};

class Vvectorint : public pVertex
{ 
private:
	vector<size_t> adj;
	size_t deg;
public:
	Vvectorint (): deg(0) {};
	void addNeighbour (const size_t v)
	{
		adj[v]++;
		deg++;
	}
	void delNeighbour (const size_t v)
	{
		if (v >= adj.size() || adj[v] == 0)
		{
			cerr << "Deleting error" << endl;
			return;
		}
		if (adj[v] > 0)
		{
			deg--;
			adj[v]--;
		}		
	}
	bool isConnect(const size_t v) const 
	{
		if (v >= adj.size())
			return false;
		return (adj[v] > 0);
	}
	size_t degree() const 
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
		for (size_t i = 0; i < adj.size(); i++)
			if (adj[i])
				cb(i);
	}
};

class Vset : public pVertex
{
private:
	set<size_t> adj;
public:
	void addNeighbour (const size_t v)
	{
		adj.insert(v);
	}
	void delNeighbour (const size_t v)
	{
		set<size_t>:: iterator it;
		it = adj.find(v);
		if (it == adj.end())
			cerr << "Deleting error" << endl;	
		adj.erase(it);
	}
	bool isConnect(const size_t v) const 
	{
		return (adj.find(v) != adj.end());
	}
	size_t degree() const 
	{
		return adj.size();
	}
	void clear()
	{
		adj.clear();
	}
	void incedents(ICB& cb)
	{
		//for_each (adj.begin(), adj.end(), cb);
		
		for (auto it = adj.begin(); it != adj.end(); it++)
			cb(*it);
	}
};

class Vmap : public pVertex
{
private:
	map<size_t, size_t> adj;
public:
	void addNeighbour (const size_t v)
	{
		adj[v]++;
	}
	void delNeighbour (const size_t v)
	{
		map<size_t, size_t>:: iterator it; // maybe static?
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
	bool isConnect(const size_t v) const 
	{
		return (adj.find(v) != adj.end());
	}
	size_t degree() const 
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
	vector<size_t> adj;
public:
	void addNeighbour (const size_t v)
	{
		adj.push_back(v);
	}
	void delNeighbour (const size_t v)
	{
		for (size_t pos = 0; pos < adj.size(); pos++)
			if (adj[pos] == v)
			{
				swap(adj[pos], adj[adj.size() - 1]);
				adj.pop_back();
				break;
			}	 
	}
	bool isConnect(const size_t v) const  
	{
		/*
		auto it = 
			find_if(adj.begin(), adj.end(), [] (int x)
			{
				return (x == v);
			});
		return (it != adj.end());*/
		
		for (auto it = adj.begin(); it != adj.end(); it++)
			if ((*it) == v)
				return true;
		return false;
	}
	size_t degree() const 
	{
		return adj.size();
	}
	void clear()
	{
		adj.clear();
	}
	void incedents(ICB& cb)
	{
		//for_each (adj.begin(), adj.end(), cb);
		
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
	void addNeighbour (const size_t v)
	{
		pV->addNeighbour(v);
	};
	void delNeighbour (const size_t v)
	{
		pV->delNeighbour(v);
	};
	const bool isConnect(const size_t v)
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