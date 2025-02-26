#include <cstring>
#include <iostream>
#include <algorithm>
#include <queue>//�ѵ�ͷ�ļ�

using namespace std;

typedef pair<int, int> PII;//����洢����ͽڵ���

const int N = 1e6 + 10;

int n, m;//�ڵ������ͱ���
int h[N], w[N], e[N], ne[N], idx;//�ڽӾ���洢ͼ
int dist[N];//�洢����
bool st[N];//�洢״̬
int pr[N];//����·�� 
int a[N];//�������·�� 
void add(int a, int b, int c)
{
    e[idx] = b; // �õ�    
	w[idx] = c; // Ȩֵ 
	ne[idx] = h[a]; // ����,���õ���һ��ָ��ԭ����ͷָ�� ͷ��ָ��õ㣬 ͷ���� 
	h[a] = idx ++ ; //ͷָ��õ� 
}

void dijkstra()
{
    memset(dist, 0x3f3f3f3f, sizeof dist);//�����ʼ��Ϊ�����
    dist[1] = 0;
    priority_queue<PII, vector<PII>, greater<PII> > heap;//С����
    heap.push({0, 1});//�������ͽڵ���

    while (heap.size())
    {
        PII t = heap.top();//ȡ����Դ������ĵ�
        heap.pop();

        int ver = t.second, distance = t.first;//ver:�ڵ��ţ�distance:Դ�����ver�ľ���

        if (st[ver]) continue;//��������Ѿ�ȷ�����������õ�
        st[ver] = true;

        for (int i = h[ver]; i != -1; i = ne[i])//����ver��ָ��Ľڵ����
        {
            int j = e[i];
            if (dist[j] > dist[ver] + w[i])
            {
                dist[j] = dist[ver] + w[i];
                pr[j] = ver; // ����·��ָ��ǰһ��·�� 
                heap.push({dist[j], j});//�����С������ѣ� ���� 
            }
        }
    }
}

int main()
{
    scanf("%d%d", &n, &m);

    memset(h, -1, sizeof h);
    while (m -- )
    {
        int a, b, c;
        scanf("%d%d%d", &a, &b, &c);
        add(a, b, c);
        //add(b, a, c); ����ʱ
    }
    dijkstra();
    
	if (dist[n] == 0x3f3f3f3f) cout << -1 << endl;
	else cout << dist[n] << endl;
	
/*	int cnt = 1, f = 0;
	a[1] = n;
	for(int i = pr[n]; i; i = pr[i]){
		a[ ++ cnt] = i;
		if(i == 1)f = 1;
	}
	if(f == 0){
	    cout << -1 << endl;
	    return 0;
	}
	for(int i = cnt; i >= 1; i -- ){
		cout << a[i];
		if(i == 1) cout << endl;
		else cout << ' ';
	}*/ //���·��
    return 0;
}

