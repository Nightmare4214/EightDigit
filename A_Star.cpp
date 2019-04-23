#include<iostream>
#include<cstdio>
#include<queue>
#include<cstring>
#include<windows.h>
using namespace std;
const char direct[4] = { 'u','d','l','r' };
const int dir[4][2] = { {-1,0},{1,0},{0,-1},{0,1} }, N = 362880;
const int aim = 46233;
const int n = 9;
bool visit[N];
int parent[N];
char step[N];
int goal_state[9][2] = {
	{0, 0}, {0, 1}, {0, 2},
	{1, 0}, {1, 1}, {1, 2},
	{2, 0}, {2, 1}, {2, 2}
};
//阶乘
const int fac[n] = { 1,1,2,6,24,120,720,5040,40320 };
//康托展开
int cantor(int s[]) {
	int result = 0, cnt = 0;
	for (int i = 0; i < n - 1; ++i) {
		cnt = 0;
		for (int j = i + 1; j < n; ++j) {
			if (s[i] > s[j])++cnt;
		}
		result += fac[n - 1 - i] * cnt;
	}
	return result;
}
//逆康托展开
void reverseCantor(int hash, int s[], int &space) {
	bool visited[n] = {};
	int temp;
	for (int i = 0; i < n; ++i) {
		temp = hash / fac[n - 1 - i];
		for (int j = 0; j < n; ++j) {
			if (!visited[j]) {
				if (temp == 0) {
					s[i] = j;
					if (j == 0)space = i;
					visited[j] = true;
					break;
				}
				--temp;
			}
		}
		hash %= fac[8 - i];
	}
}
int abs(int a, int b) {
	if (a > b)return a - b;
	return b - a;
}
//f=g+h
int f[N];
int g[N];
//曼哈顿距离
int h(int s[]) {
	int k, result = 0;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			k = i * 3 + j;
			if (s[k] == 0)continue;
			//oj上abs(1)会同时匹配abs(int)和abs(double)
			result += abs(i, goal_state[s[k] - 1][0]) + abs(j, goal_state[s[k] - 1][1]);
		}
	}
	return result;
}
struct cmp {
	bool operator ()(const int &a, const int &b) {
		if (f[a] == f[b])return g[a] > g[b];
		return f[a] > f[b];
	}
};
void printPath() {
	char queue[N];
	int t = 0;
	int c = aim;
	while (parent[c] != -1) {
		queue[t] = step[c];
		++t;
		c = parent[c];
	}
	for (int i = t - 1; i >= 0; --i)printf("%c", queue[i]);
	printf("\n");
}
//A*算法
void A_star(int start) {
	priority_queue<int, vector<int>, cmp> q;
	q.push(start);
	int preHash, hash, state[n], space;
	while (!q.empty()) {
		//取出节点
		preHash = q.top();
		if (preHash == aim) {
			printPath();
			return;
		}
		q.pop();
		reverseCantor(preHash, state, space);
		for (int i = 0; i < 4; ++i) {
			int tx = space / 3 + dir[i][0];
			int ty = space % 3 + dir[i][1];
			if (tx < 0 || tx>2 || ty < 0 || ty>2)continue;
			int tz = 3 * tx + ty;
			state[space] = state[tz];
			state[tz] = 0;
			hash = cantor(state);
			//没访问过
			if (!visit[hash]) {
				step[hash] = direct[i];
				g[hash] = g[preHash] + 1;
				visit[hash] = true;
				parent[hash] = preHash;
				f[hash] = g[hash] + h(state);
				q.push(hash);
			}
			//访问过了，但是从preHash走一步到hash比直接走到hash更短
			else if (g[preHash] + 1 < g[hash]) {
				//貌似并不会进来
				step[hash] = direct[i];
				f[hash] = f[hash] - g[hash] + g[preHash] + 1;
				g[hash] = g[preHash] + 1;
				parent[hash] = preHash;
				q.push(hash);
			}
			state[tz] = state[space];
			state[space] = 0;
		}
	}
}
int getInv(int s[]) {
	int cnt = 0;
	for (int i = 1; i < n; ++i) {
		if (s[i] == 0)continue;
		for (int j = 0; j < i; ++j) {
			if (s[j] == 0)continue;
			if (s[i] < s[j])
				++cnt;
		}
	}
	return cnt & 1;
}
int main() {
	char x;
	int a[n];
	while (~scanf(" %c", &x)) {
		if (x == 'x')a[0] = 0;
		else a[0] = x - '0';
		for (int i = 1; i < n; ++i) {
			scanf(" %c", &x);
			if (x == 'x')a[i] = 0;
			else a[i] = x - '0';
		}
		LARGE_INTEGER m_nFreq;
		LARGE_INTEGER m_nBeginTime;
		LARGE_INTEGER nEndTime;
		QueryPerformanceFrequency(&m_nFreq); // 获取时钟周期
		QueryPerformanceCounter(&m_nBeginTime); // 获取时钟计数
		if (getInv(a) == 1) {
			printf("unsolvable\n");
			QueryPerformanceCounter(&nEndTime);
			cout << (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << "ms" << endl;
			continue;
		}
		int hash = cantor(a);
		memset(visit, 0, sizeof(visit));
		memset(g, 0, sizeof(g));
		memset(f, 0, sizeof(f));
		visit[hash] = true;
		parent[hash] = -1;
		g[hash] = 0;
		f[hash] = h(a);
		A_star(hash);
		cout << g[aim] << endl;
		QueryPerformanceCounter(&nEndTime);
		cout << (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << "ms" << endl;
	}
	return 0;
}