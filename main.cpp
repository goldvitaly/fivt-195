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
	IntervalTree<LL,LL,gcd,multiplyIgnore,std::multiplies<LL> > tree(v.begin(),v.end(),0,1);
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

		//cout<<"count " <<l<<" "<<r<<" ans= "<<ans<<" & found "<<tree.get(l,  r)<<endl;
		assert(ans == tree.get(l,  r));

	}
}
/*
class SegmentsCount{
	struct resType{
		int l,r;
		size_t cnt;
		int time;
		resType():l(0),r(0),cnt(1),time(0){}
		resType(int l,int r,size_t cnt, int time=0):l(l),r(r),cnt(cnt),time(time){}
	};
	struct mergeRes{
		resType operator()(const resType& a,const resType &b) const{
			return resType(a.l, b.r, a.cnt+b.cnt - (a.r==b.l ? 1 : 0),max(a.time,b.time));
		}
	};
	struct edit{
		int value;
		int time;
		edit():value(0),time(-1){}
		edit(int value,int time):value(value), time(time){}
	};
	struct modRes{
		resType operator()(const resType& res, const edit& edit, size_t) const{
			if(res.time >= edit.time)
				return res;
			else
				return resType(edit.value, edit.value, 1, edit.time);
		}
	};
	struct secondEdit{
		edit operator()(const edit& a,const edit& b) const{
			return b;
		}
	};
	
	IntervalTree<resType,edit,mergeRes,modRes,secondEdit> tree;
	unsigned time;
public:
	explicit SegmentsCount(int n): tree(n),time(1){
		
	}

	void set(size_t l,size_t r,int color){
		tree.set(l,r,edit(color,time++));
	}
	int get(size_t l,size_t r) const{
		return tree.get(l,r).cnt;
	}
};
/*
void test_segment_count(int n,int times){
	SegmentsCount tree(n);
	srand(3);
	vector<LL> v(n);
	for(int i=0;i<times;++i){
		int l=rand()%n;
		int r=rand()%n;
		LL value=rand();
		if(l>r)
			swap(l, r);
		for(int i=l;i<=r;++i){
			v[i] = value;
		}
		tree.set(l, r, value);
		cout<<"setted "<<l<<" "<<r<<" to "<<value<<endl;

		for(int i=0;i<n;++i){
			for(int j=i;j<n;++j){
				cout<<i<<j<<' '<<(tree.get(i,j))<<endl;
			}
		}

		l=rand()%n;
		r=rand()%n;
		if(l>r)
			swap(l, r);

		int ans = 1;

		for(int i=l+1;i<=r;++i){
			if(v[i]!=v[i-1])
				++ans;
		}

		cout<<"count " <<l<<" "<<r<<" ans= "<<ans<<" & found "<<tree.get(l,  r)<<endl;
		assert(ans == tree.get(l,  r));

	}

}*/
int main() {
	//cout<<"x";
	test_sum(10000,10000);
	test_prefilling(10,10000);
	test_gcd(1000,100);
	//test_segment_count(10,10000);
	return 0;
}

