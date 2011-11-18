#include "IntervalTree.h"
#include <cstdlib>
using namespace std;
typedef long long LL;
struct addSegment{
	bool operator()(LL old, LL mod, LL len){
		return old+mod*len;
	}
};
void test_sum(int n,int times){
	srand(1);
	IntervalTree<LL,LL,std::plus<LL>,addSegment,std::plus<LL> > tree(n);
	vector<LL> v(n);
	for(int i=0;i<times;++i){
		int l=rand()%n;
		int r=rand()%n;
		int value=rand();
		if(l>r)
			swap(l, r);
		for(int i=l;i<=r;++i){
			v[i] += value;
		}
		tree.set(l, r, value);

		l=rand()%n;
		r=rand()%n;
		if(l>r)
			swap(l, r);

		LL ans = 0;

		for(int i=l;i<=r;++i){
			ans+=v[i];
		}
		assert(ans == tree.get(l,  r));

	}
}
int main(int argc, char** argv) {
	test_sum(1000,1000);
	return 0;
}

