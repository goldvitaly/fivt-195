#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
#define forn(i, n) for (int i = 0; i < (int)(n); i++)
const int maxn = 20050;
#define pb push_back

int n, m;
vector<int> e[maxn];

int col[maxn];
int st[maxn], *pst = st;
int b[maxn];
int tin[maxn], tup[maxn];
int dtime = 0;
int ncomp = 0;

int readInt()
{
	char c = ' ';
	while (c == ' ' || c == '\n')
		c = getchar();
	int res = 0;
	do
	{
		res = res * 10 + c - '0';
		c = getchar();
	}
	while (c != ' ' && c != '\n');
	return res;
}
void writeInt(int x)
{
	static char s[10] = {0,0,0,0,0,0,0,0,0,0};
	for (int i = 8; i >= 0; i--)
	{
		if (!x) s[i] = 0;
		else s[i] = x%10 + '0';
		x /= 10;
	}
	char* s1 = s;
	while (!*s1)
		++s1;
	puts(s1);
}

void scan()
{
	n = readInt();
	m = readInt();
	forn(i, m)
	{
		int u, v;
		u = readInt();
		v = readInt();
		e[--u].pb(--v);
	}
}
void dfs(int, int);

void solve()
{
	forn(i, n) col[i] = -1;
	forn(i, n) if (!b[i]) dfs(i, -1);
	writeInt(ncomp);
	forn(i, n) writeInt(ncomp - col[i]);
}

void dfs(int v, int anc)
{
	b[v] = 1;
	*(++pst) = v;
	tin[v] = tup[v] = dtime++;
	forn(i, e[v].size())
	{
		int to = e[v][i];
		if (col[to] != -1)
			continue;
		if (b[to])
			tup[v] = min(tup[v], tin[to]);
		else
		{
			dfs(to, v);
			tup[v] = min(tup[v], tup[to]);
		}
	}
	if (tup[v] == tin[v])
	{
		while (pst != st && tin[*pst] >= tin[v])
			col[*(pst--)] = ncomp;
		++ncomp;
	}	
}

int main()
{
	freopen("input.txt", "r", stdin);
	scan();
	solve();
	
	return 0;
}
