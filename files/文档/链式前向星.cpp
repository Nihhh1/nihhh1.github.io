#include<bits/stdc++.h>
using namespace std;
const int maxn = 1005;//�������ֵ
int n, m, cnt;//n���㣬m����
struct Edge
{
    int to, w, next;//�յ㣬��Ȩ��ͬ������һ���ߵı��
}edge[maxn];//�߼�
int head[maxn];//head[i],��ʾ��iΪ���ĵ�һ�����ڱ߼������λ�ã���ţ�
void init()//��ʼ��
{
    for (int i = 0; i <= n; i++) head[i] = -1;
    cnt = 0;
}
void add_edge(int u, int v, int w)//�ӱߣ�u��㣬v�յ㣬w��Ȩ
{
    edge[cnt].to = v; //�յ�
    edge[cnt].w = w; //Ȩֵ
    edge[cnt].next = head[u];//��uΪ�����һ���ߵı�ţ�Ҳ����������������ͬ����һ���ߵı��
    head[u] = cnt++;//������uΪ�����һ���ߵı��
}
int main()
{
    cin >> n >> m;
    int u, v, w;
    init();//��ʼ��
    for (int i = 1; i <= m; i++)//����m����
    {
        cin >> u >> v >> w;
        add_edge(u, v, w);//�ӱ�
    }
}
