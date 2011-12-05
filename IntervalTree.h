/* 
 * File:   IntervalTree.h
 * Author: riad
 *
 * Created on November 14, 2011, 4:28 PM
 */

#ifndef INTERVALTREE_H
#define INTERVALTREE_H
#include <cassert>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <boost/optional.hpp>
template <typename Element, typename Modification, typename MergeFunctor,
	typename ModFunc, typename CalcMod>
class IntervalTree{
	struct Interval{
		int l,r;
		Interval(int l,int r):l(l),r(r){}
		Interval leftPart() const {
			return Interval(l, (l+r)>>1);
		}

		Interval rightPart() const {
			return Interval(((l+r)>>1)+1,r);
		}

		bool partOf(const Interval& b) const {
			return l>=b.l && r<=b.r;
		}

		bool intersect(const Interval& b) const {
			return r>=b.l && l<=b.r;
		}

		size_t length() const {
			return r - l + 1;
		}

		Interval intersection(const Interval& b) const {
			assert(intersect(b));
			return Interval(std::max(l, b.l),std::min(r, b.r));
		}
	};
	std::vector<Element> tree;
	std::vector<boost::optional<Modification> > mod;
	size_t shift;
	MergeFunctor functor;
	ModFunc modify;
	CalcMod calcMod;
	Element Zero;
	size_t size;
	void allocate(size_t n){
		assert(n>0);
		size = n;
		size_t height=0;
		while(n>(1<<height)){
			++height;
		}
		shift = 1<<height;

		tree.assign(1<<(height+1),Zero);
		mod.assign(1<<(height+1),boost::optional<Modification>());
	}

	Element apply_modification(const Element& element, const boost::optional<Modification>& modification, size_t len) const{
		if(modification)
			return modify(element, *modification, len);
		else
			return element;
	}
	void add_modification(size_t v,const Modification& modification){
		if(mod[v])
			mod[v] = calcMod(*mod[v], modification);
		else
			mod[v] = modification;
	}

	void recalc(size_t pos,size_t len){
		assert(pos<shift);
		tree[pos]=functor(apply_modification(tree[2*pos],mod[2*pos],len>>1),
		                  apply_modification(tree[2*pos+1],mod[2*pos+1],len>>1));
	}

	void mod_down(size_t v, size_t len){
		if(!mod[v])
			return;
		if(v>=shift){
			tree[v] = modify(tree[v], *mod[v], len);
		}
		else{
			add_modification(2*v, *mod[v]);
			add_modification(2*v+1, *mod[v]);
			recalc(v, len);
		}
		mod[v].reset();
	}
	
	Element _get(size_t v,Interval query,Interval vertex){
		if(!query.intersect(vertex))
			return Zero;
		mod_down(v, vertex.length());
		if(vertex.partOf(query))
			return tree[v];
		assert(v<shift);
		return functor(
			_get(2*v, query, vertex.leftPart()),
			_get(2*v+1, query, vertex.rightPart())
		);
	}

	void _set(size_t v, Interval query, Interval vertex,Modification arg){
		if(!vertex.intersect(query))
			return;
		if(vertex.partOf(query)){
			add_modification(v, arg);
			return;
		}
		mod_down(v, vertex.length());
		assert(v<shift);
		_set(2*v  , query, vertex.leftPart() , arg);
		_set(2*v+1, query, vertex.rightPart(), arg);
		recalc(v, vertex.length());
	}

	public:
	explicit IntervalTree(size_t n, Element Zero = Element(),
			MergeFunctor functor = MergeFunctor(),
			ModFunc modify=ModFunc(), CalcMod calcMod=CalcMod()):
			functor(functor), modify(modify), calcMod(calcMod),Zero(Zero)
	{
		allocate(n);
		
	}

	template <typename Iterator>
	IntervalTree(Iterator begin,Iterator end,
			Element Zero = Element(),
			MergeFunctor functor = MergeFunctor(),
			ModFunc modify=ModFunc(), CalcMod calcMod=CalcMod()):
			functor(functor), modify(modify), calcMod(calcMod),Zero(Zero)
	{
		allocate(end-begin);
		std::copy(begin, end, tree.begin()+shift);


		int nextLevel = shift;
		int len = 1;
		for(int i=shift-1;i>0;--i){
			if(i<nextLevel){
				nextLevel>>=1;
				++len;
			}
			recalc(i,len);
		}
	}

	Element get(size_t left, size_t right){
		if(left>right || right>=size)
			throw std::logic_error("Invalid range");
		return _get(1, Interval(left, right),Interval(0,shift -1));
	}

	void set(size_t left, size_t right, Modification arg){
		if(left>right || right>=size)
			throw std::logic_error("Invalid range");
		_set(1, Interval(left, right), Interval(0, shift - 1), arg);
	}
	
};

#endif	/* INTERVALTREE_H */

