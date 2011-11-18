/* 
 * File:   IntervalTree.h
 * Author: riad
 *
 * Created on November 14, 2011, 4:28 PM
 */

#ifndef INTERVALTREE_H
#define	INTERVALTREE_H
#include <cassert>
#include <iostream>
#include <vector>
#include <stdexcept>
template <typename ElementType, typename ModType, typename MergeFunctorType,
	typename ModFuncType, typename CalcModType,ElementType Zero = ElementType(),
	ModType ModZero = ModType() >
class IntervalTree{
	std::vector<ElementType> tree;
	std::vector<ModType> mod;
	size_t shift;
	MergeFunctorType functor;
	ModFuncType modify;
	CalcModType calc_mod;
	void allocate(size_t n){
		assert(n>0);
		size_t height=0;
		while(n>(1<<height)){
			++height;
		}
		shift = 1<<height;

		tree.assign(1<<(height+1),Zero);
		mod.assign(1<<(height+1),ModZero);
		
	}

	void recalc(size_t pos,int len){
		assert(pos<shift);
		tree[pos]=functor(modify(tree[2*pos],mod[2*pos],len>>1),modify(tree[2*pos+1],mod[2*pos+1],len>>1));
	}

	ElementType _get(size_t v,size_t l,size_t r,size_t v_l,size_t v_r){
		if(l<=v_l && r>=v_r)
			return modify(tree[v], mod[v], v_r-v_l+1);
		if(l>r)
			return Zero;
		assert(v<shift);
		return functor(
			_get(2*v, l, r, v_l, (v_l+v_r)>>1),
			_get(2*v+1, l, r, (v_l+v_r)>>1+1,v_r)
		);
	}

	void _set(size_t v, size_t l,size_t r, size_t v_l,size_t v_r,ModType arg){
		if(l>=v_l && r<=v_r){
			mod[v] = calc_mod(mod[v], arg);
			return;
		}
		if(l>v_r || r<v_l)
			return;
		assert(v<shift);
		_set(2*v  , l, r, v_l, (v_l+v_r)>>1  , arg);
		_set(2*v+1, l, r, (v_l+v_r)>>1+1, v_r, arg);
		recalc(v, v_r - v_l+1);
	}

	public:
	explicit IntervalTree(size_t n, MergeFunctorType functor = MergeFunctorType(),
			ModFuncType modify=ModFuncType(), CalcModType calc_mod=CalcModType()):
			functor(functor), modify(modify), calc_mod(calc_mod)
	{
		allocate(n);
	}

	template <typename Iterator>
	IntervalTree(Iterator begin, Iterator end, MergeFunctorType functor = MergeFunctorType(),
			ModFuncType modify=ModFuncType(), CalcModType calc_mod=CalcModType()):
			functor(functor), modify(modify), calc_mod(calc_mod)
	{
		allocate(end-begin);
		std::copy(begin, end, tree.begin()+1);


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

	ElementType get(size_t left, size_t right){
		if(left>right)
			throw std::logic_error("Invalid range");
		return _get(1, left, right, 0, shift-1);
	}

	void set(size_t left, size_t right, ModType arg){
		if(left>right)
			throw std::logic_error("Invalid range");
		_set(1, left, right, 0, shift - 1, arg);
	}
	
};

#endif	/* INTERVALTREE_H */

