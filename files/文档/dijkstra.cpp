#include<iostream>
#include<algorithm>
#include<cstring>
using namespace std;

const int N=510;

int g[N][N];    //Ϊ�������������ڽӾ���洢
int dist[N];    //���ڼ�¼ÿһ��������һ����ľ���
bool st[N];     //���ڼ�¼�õ����̾����Ƿ��Ѿ�ȷ��

int n,m;

int Dijkstra() 
{
    memset(dist, 0x3f,sizeof dist);     //��ʼ������  0x3f�������޴�

    dist[1]=0;  //��һ���㵽����ľ���Ϊ0

    for(int i=0;i<n;i++)      //��n��������Ҫ����n�� ����
    {
        int t=-1;       //t�洢��ǰ���ʵĵ�

        for(int j=1;j<=n;j++)   //�����j������Ǵ�1�ŵ㿪ʼ
            if(!st[j]&&(t==-1||dist[t]>dist[j]))     
                t=j;

        st[t]=true;   

        for(int j=1;j<=n;j++)           //���θ���ÿ�����������ڵĵ�·��ֵ
            dist[j]=min(dist[j],dist[t]+g[t][j]);
    }

    if(dist[n]==0x3f3f3f3f) return -1;  //�����n����·��Ϊ����󼴲��������·��
    return dist[n];
}
int main()
{
    cin>>n>>m;

    memset(g,0x3f,sizeof g);    //��ʼ��ͼ ��Ϊ�������·�� ����ÿ�����ʼΪ���޴�

    while(m--)
    {
        int x,y,z;
        cin>>x>>y>>z;
       g[y][x]= g[x][y]=min(g[x][y],z);     //��������رߵ����������̵�һ����
    }

    cout<<Dijkstra()<<endl; 
    return 0;
}
