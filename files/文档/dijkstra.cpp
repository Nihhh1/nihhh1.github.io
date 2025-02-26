#include<iostream>
#include<algorithm>
#include<cstring>
using namespace std;

const int N=510;

int g[N][N];    //为稠密阵所以用邻接矩阵存储
int dist[N];    //用于记录每一个点距离第一个点的距离
bool st[N];     //用于记录该点的最短距离是否已经确定

int n,m;

int Dijkstra() 
{
    memset(dist, 0x3f,sizeof dist);     //初始化距离  0x3f代表无限大

    dist[1]=0;  //第一个点到自身的距离为0

    for(int i=0;i<n;i++)      //有n个点所以要进行n次 迭代
    {
        int t=-1;       //t存储当前访问的点

        for(int j=1;j<=n;j++)   //这里的j代表的是从1号点开始
            if(!st[j]&&(t==-1||dist[t]>dist[j]))     
                t=j;

        st[t]=true;   

        for(int j=1;j<=n;j++)           //依次更新每个点所到相邻的点路径值
            dist[j]=min(dist[j],dist[t]+g[t][j]);
    }

    if(dist[n]==0x3f3f3f3f) return -1;  //如果第n个点路径为无穷大即不存在最低路径
    return dist[n];
}
int main()
{
    cin>>n>>m;

    memset(g,0x3f,sizeof g);    //初始化图 因为是求最短路径 所以每个点初始为无限大

    while(m--)
    {
        int x,y,z;
        cin>>x>>y>>z;
       g[y][x]= g[x][y]=min(g[x][y],z);     //如果发生重边的情况则保留最短的一条边
    }

    cout<<Dijkstra()<<endl; 
    return 0;
}
