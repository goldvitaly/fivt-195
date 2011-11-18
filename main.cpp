#include "IntervalTree.h"
#include <cstdlib>
using namespace std;
typedef long long LL;
struct addSegment{
	LL operator()(LL old, LL mod, size_t len) const{
		return old+mod*len;
	}
};
//Сумма на отрезке.
//Увеличение/уменьшение каждого чиал на отрезке.
void test_sum(int n,int times){
	srand(1);
	IntervalTree<LL,LL,std::plus<LL>,addSegment,std::plus<LL> > tree(n);
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
		tree.set(l, r, value);
		//cout<<"setted "<<l<<" "<<r<<" to "<<value<<endl;
		l=rand()%n;
		r=rand()%n;
		if(l>r)
			swap(l, r);

		LL ans = 0;

		for(int i=l;i<=r;++i){
			ans+=v[i];
		}

		//cout<<"count " <<l<<" "<<r<<" ans= "<<ans<<" & found "<<tree.get(l,  r)<<endl;
		assert(ans == tree.get(l,  r));

	}
}
void test_prefilling(int n, int times){
	
	vector<LL> v(n);
	for(int i=0;i<n;++i){
		v[i]=rand();
	}
	IntervalTree<LL,LL,std::plus<LL>,addSegment,std::plus<LL> > tree(v.begin(),v.end());

	for(int i=0;i<times;++i){
		int l=rand()%n;
		int r=rand()%n;
		if(l>r)
			swap(l, r);

		LL ans = 0;

		for(int i=l;i<=r;++i){
			ans+=v[i];
		}

		//cout<<"count " <<l<<" "<<r<<" ans= "<<ans<<" & found "<<tree.get(l,  r)<<endl;
		assert(ans == tree.get(l,  r));

	}

}
struct gcd{
	LL operator()(LL a,LL b) const{
		if(!b)
			return a;
		return (*this)(b,a%b);
	}
};
//Не рекомендуется большое times, т.к возможно переполнение
// НОД на отрезке, умножение каждого числа на отрезке
struct multiplyIgnore{
	LL operator()(LL a, LL b, size_t)  const{
		return a*b;
	}
};
void test_gcd(int n,int times){
	srand(2);
	
	vector<LL> v(n);
	for(int i=0;i<n;++i){
		v[i]=rand()%100;
	}
	IntervalTree<LL,LL,gcd,multiplyIgnore,std::multiplies<LL>,0,1 > tree(v.begin(),v.end());
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
		//cout<<"setted "<<l<<" "<<r<<" to "<<value<<endl;
		l=rand()%n;
		r=rand()%n;
		if(l>r)
			swap(l, r);

		LL ans = 0;

		for(int i=l;i<=r;++i){
			ans=gcd()(ans,v[i]);
		}

		cout<<"count " <<l<<" "<<r<<" ans= "<<ans<<" & found "<<tree.get(l,  r)<<endl;
		assert(ans == tree.get(l,  r));

	}
}
int main(int argc, char** argv) {
	test_sum(10000,10000);
	test_prefilling(10,10000);
	test_gcd(10,100);
	return 0;
}

