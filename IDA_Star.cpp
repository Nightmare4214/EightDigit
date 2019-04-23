#include<iostream>
#include<cstdio>
#include<windows.h>
using namespace std;
const char direct[4] = { 'u','d','l','r' };
const int dir[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };
const int n = 9;
bool flag;
int a[n];
char step[32];
int maxDeep;
int goal_state[9][2] = {
	{0, 0}, {0, 1}, {0, 2},
	{1, 0}, {1, 1}, {1, 2},
	{2, 0}, {2, 1}, {2, 2}
};
int abs(int a, int b) {
	if (a > b)return a - b;
	return b - a;
}
int h() {
	int k, result = 0;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			k = i * 3 + j;
			if (a[k] == 0)continue;
			result += abs(i, goal_state[a[k] - 1][0]) + abs(j, goal_state[a[k] - 1][1]);
		}
	}
	return result;
}
int getInv() {
	int cnt = 0;
	for (int i = 1; i < n; ++i) {
		if (a[i] == 0)continue;
		for (int j = 0; j < i; ++j) {
			if (a[j] == 0)continue;
			if (a[i] < a[j])
				++cnt;
		}
	}
	return cnt & 1;
}
bool reverse(char a, char b) {
	return a == 'u'&& b == 'd'
		|| a == 'd'&& b == 'u'
		|| a == 'l'&& b == 'r'
		|| a == 'r'&&b == 'l';
}
int dfs(int deep, int space) {
	int t = h(), f = deep + t;
	if (f > maxDeep)return f;
	if (t == 0) {
		flag = true;
		return f;
	}
	int nextBound = 32;
	for (int i = 0; i < 4; ++i) {
		if (deep > 0 && reverse(step[deep - 1], direct[i]))continue;
		int tx = space / 3 + dir[i][0];
		int ty = space % 3 + dir[i][1];
		if (tx < 0 || tx>2 || ty < 0 || ty>2)continue;
		int tz = 3 * tx + ty;
		a[space] = a[tz];
		a[tz] = 0;
		step[deep] = direct[i];
		f = dfs(deep + 1, tz);
		if (flag)return f;
		if (f < nextBound)nextBound = f;
		a[tz] = a[space];
		a[space] = 0;
	}
	return nextBound;
}
int main() {
	char x;
	int space;
	while (~scanf(" %c", &x)) {
		if (x == '0') {
			a[0] = 0;
			space = 0;
		}
		else a[0] = x - '0';
		for (int i = 1; i < n; ++i) {
			scanf(" %c", &x);
			if (x == '0') {
				a[i] = 0;
				space = i;
			}
			else a[i] = x - '0';
		}
		LARGE_INTEGER m_nFreq;
		LARGE_INTEGER m_nBeginTime;
		LARGE_INTEGER nEndTime;
		QueryPerformanceFrequency(&m_nFreq); // 获取时钟周期
		QueryPerformanceCounter(&m_nBeginTime); // 获取时钟计数
		if (getInv() == 1) {
			printf("unsolvable\n");
		}
		else {
			maxDeep = h();
			while (true) {
				flag = false;
				maxDeep = dfs(0, space);
				if (flag) {
					step[maxDeep] = '\0';
					printf("%s\n", step);
					printf("%d\n", maxDeep);
					break;
				}
			}

		}
		QueryPerformanceCounter(&nEndTime);
		cout << (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << "ms" << endl;
	}
	return 0;
}