#include "IntervalTree.h"
#include "MinimizeTreeSetModification.h"
#include "RSQAddModification.h"
#include <cstdlib>
#include <iostream>
#include <limits>
using namespace std;
typedef long long LL;
struct addSegment{
	LL operator()(LL old, LL mod, size_t len) const {
		return old+mod*len;
	}
};
void test_sum(int n,int times){
	srand(1);
	RSQAddModification<LL> tree(n);
	vector<LL> v(n);
	for(int i=0;i<times;++i){
		int l=rand()%n;
		int r=rand()%n;
		LL value=rand();
		if(l>r)
			swap(l, r);
		for(int i=l;i<=r;++i){
			v[i] += value;
		}
		tree.increase(l, r, value);

		l=rand()%n;
		r=rand()%n;
		if(l>r)
			swap(l, r);

		LL ans = 0;
		for(int i=l;i<=r;++i){
			ans+=v[i];
		}
		assert(ans == tree.getSum(l,  r));
	}
}
void test_prefilling(int n, int times){
	srand(3);
	vector<LL> v(n);
	for(int i=0;i<n;++i){
		v[i]=rand();
	}
	RSQAddModification<LL> tree(v.begin(),v.end());
	for(int i=0;i<times;++i){
		int l=rand()%n;
		int r=rand()%n;
		if(l>r)
			swap(l, r);
		LL ans = 0;
		for(int i=l;i<=r;++i){
			ans+=v[i];
		}
		assert(ans == tree.getSum(l,  r));
	}

}
struct gcd{
	LL operator()(LL a,LL b) const{
		if(!b)
			return a;
		return (*this)(b,a%b);
	}
};
struct multiplyIgnore{
	LL operator()(LL a, LL b, size_t)  const{
		return a*b;
	}
};

//Не рекомендуется большое times, т.к возможно переполнение
// НОД на отрезке, умножение каждого числа на отрезке
void test_gcd(int n,int times){
	srand(2);
	
	vector<LL> v(n);
	for(int i=0;i<n;++i){
		v[i]=rand()%100;
	}
	IntervalTree<LL,LL,gcd,multiplyIgnore,multiplies<LL> > tree(v.begin(),v.end());
	for(int i=0;i<times;++i){
		int l=rand()%n;
		int r=rand()%n;
		LL value=rand()%10;
		if(l>r)
			swap(l, r);
		for(int i=l;i<=r;++i){
			v[i] *= value;
		}
		tree.set(l, r, value);

		l=rand()%n;
		r=rand()%n;
		if(l>r)
			swap(l, r);

		LL ans = 0;

		for(int i=l;i<=r;++i){
			ans=gcd()(ans,v[i]);
		}
		assert(ans == tree.get(l,  r));
	}
}
template <typename T>
void test_min(size_t n,int times){
	srand(4);
	MinimizeTreeSetModification<T> tree(n,17);
	vector<int> v(n,17);
	for(int i=0;i<times;++i){
		int l=rand()%n;
		int r=rand()%n;
		int value=rand()%10;
		if(l>r)
			swap(l, r);
		for(int i=l;i<=r;++i){
			v[i] = value;
		}
		tree.setValue(l, r, value);
		//cout<<"setted "<<l<<" "<<r<<" to "<<value<<endl;
		l=rand()%n;
		r=rand()%n;
		if(l>r)
			swap(l, r);

		int ans = numeric_limits<int>::max();

		for(int i=l;i<=r;++i){
			ans=min(ans,v[i]);
		}
		assert(ans == tree.getMin(l,  r));

	}
}
int main() {
	test_sum(10000,10000);
	test_prefilling(1000,10000);
	test_gcd(1000,100);
	test_min<int>(10000,10000);
	return 0;
}

