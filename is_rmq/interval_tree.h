#ifndef INTERVAL_TREE_H
#define INTERVAL_TREE_H

#include <iostream>
#include <limits>

#ifndef NULL
#define NULL 0L
#endif

namespace rq{

template<typename T>
class min
{
public:
	const T& getNeutral() const
	{
		const static T tmp = std :: numeric_limits<T> :: max();
		return tmp;
	}
	const T& operator() (const T& a, const T& b) const
	{
		return std :: min(a, b);
	}

	void reset (T& val) const
	{
		val = getNeutral();
	}
};

template<typename T>
class minPainter
{
private:
	const static T neutral = 0;
	int left, right;
	
public:
	minPainter (int _left, int _right)
	{
		left = _left;
		right = _right;
	}
	const T& merge (const T& val, const T& mod) const
	{
		return mod == neutral ? val : mod;
	}
	void mergeMod (const T& mod1, T& mod2) const
	{
		if (mod1 != neutral)
			mod2 = mod1;
	}
	void reset (T& val) const
	{
		val = neutral;
	}
};



template<typename T>
class sum
{
public:
	T getNeutral() const
	{
		return 0;
	}
	T operator() (const T& a, const T& b) const
	{
		return a + b;
	}

	void reset (T& val) const
	{
		val = 0;
	}
};

template<typename T>
class sumAdder
{
private:
	const static T neutral = 0;
	int left, right;
	
public:
	sumAdder (int _left, int _right)
	{
		left = _left;
		right = _right;
	}
	T merge (const T& val, const T& mod) const
	{
		return val + mod * (right - left + 1);
	}
	void mergeMod (const T& mod1, T& mod2) const
	{
		mod2 += mod1;
	}
	void reset (T& val) const
	{
		val = neutral;
	}
};

template<typename T, typename Func, typename TMod, typename Modder>
class node
{
private:
	int left, right;
	node *lChild, *rChild;
	
	T val;
	TMod mod;
	Modder modder;
	Func func;
	
	T mergedValue()
	{
		return modder.merge(val, mod);
	}
	
	void update()
	{
		//push
		if (lChild)
		{
			modder.mergeMod(mod, lChild->mod);
			modder.mergeMod(mod, rChild->mod);
		}
		else
			val = modder.merge(val, mod);
		modder.reset(mod);
		
		//update
		if (lChild)
		{
			val = func( lChild->mergedValue(),
						rChild->mergedValue() );
		}
	}
	
public: 	
	node (int _left, int _right): modder(_left, _right), func()
	{
		left  = _left;
		right = _right;
		func.reset(val);
		modder.reset(mod);
		if (left == right)
			lChild = rChild = NULL;
		else
		{
			lChild = new node(left, (left + right) / 2);
			rChild = new node((left + right) / 2 + 1, right);
		}
	}
	
	T get (int qLeft, int qRight)
	{
		update();
		if (qLeft <= left && qRight >= right)
			return val;
		if (qLeft > right || qRight < left)
			return func.getNeutral();
		return func( lChild->get( qLeft, qRight ),
					 rChild->get( qLeft, qRight ) );
	}
	
	void modify (int qLeft, int qRight, TMod newVal)
	{
		update();
		if (qLeft <= left && qRight >= right)
			modder.mergeMod(newVal, mod);
		else if (qLeft > right || qRight < left)
			return;
		else
		{
			lChild->modify(qLeft, qRight, newVal);
			rChild->modify(qLeft, qRight, newVal);
			update();
		}
	}
};

template<typename T, typename Func, typename TMod, typename Modder>
class RangeQuery
{
private:
	node<T, Func, TMod, Modder> *base;
	
public:
	RangeQuery(int lq, int rq)
	{
		base = new node<T, Func, TMod, Modder>(lq, rq);
	}
	T get (int lq, int rq)
	{
		return base->get(lq, rq);
	}
	void update(int lq, int rq, TMod x)
	{
		base->modify(lq, rq, x);
	}
};

} //namespace rq
#endif
