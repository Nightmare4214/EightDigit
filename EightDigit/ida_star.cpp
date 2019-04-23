#include<iostream>
#include<cstring>
using namespace std;
//对应的界面的走法
const int direct[4][2]={{0,-1},{0,1},{-1,0},{1,0}};
//走法
const int dir[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
const int n = 9;
bool flag;
//棋盘
int a[n];
//步骤
int step[32];
//搜索上界
int maxDeep;
//目标状态
const int goal_state[9][2] = {
    {0, 0}, {0, 1}, {0, 2},
    {1, 0}, {1, 1}, {1, 2},
    {2, 0}, {2, 1}, {2, 2}
};
int abs(int a, int b) {
    if (a > b)return a - b;
    return b - a;
}
//曼哈顿距离作为评估函数
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
//获得棋盘逆序数
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
int dfs(int deep, int space) {
    int t = h(), f = deep + t;
    if (f > maxDeep)return f;
    if (t == 0) {
        flag = true;
        return f;
    }
    int nextBound = 32;
    for (int i = 0; i < 4; ++i) {
        //不能和上一步相反
        if (deep > 0 && (1==(step[deep - 1]^i)))continue;
        int tx = space / 3 + dir[i][0];
        int ty = space % 3 + dir[i][1];
        if (tx < 0 || tx>2 || ty < 0 || ty>2)continue;
        int tz = 3 * tx + ty;
        a[space] = a[tz];
        a[tz] = 0;
        step[deep] = i;
        f = dfs(deep + 1, tz);
                if (flag)return f;
                if (f < nextBound)nextBound = f;
        a[tz] = a[space];
        a[space] = 0;
    }
    return nextBound;
}
//ida*驱动函数
void IDA_star(int b[],int space,int temp_step[][2],int &step_size){
    memcpy(a,b,sizeof(int)*9);
    maxDeep = h();
    while (true) {
        flag = false;
        maxDeep = dfs(0, space);
        if (flag) {
            for(int i=0;i<maxDeep;++i){
                temp_step[i][0]=direct[step[i]][0];
                temp_step[i][1]=direct[step[i]][1];

            }
            step_size=maxDeep;
            break;
        }
    }
}
