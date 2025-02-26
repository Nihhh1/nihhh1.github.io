#include <cstring>
#include <iostream>
#include <algorithm>
#include <queue>//堆的头文件

using namespace std;

typedef pair<int, int> PII;//堆里存储距离和节点编号

const int N = 1e6 + 10;

int n, m;//节点数量和边数
int h[N], w[N], e[N], ne[N], idx;//邻接矩阵存储图
int dist[N];//存储距离
bool st[N];//存储状态
int pr[N];//储存路径 
int a[N];//辅助输出路径 
void add(int a, int b, int c)
{
    e[idx] = b; // 该点    
	w[idx] = c; // 权值 
	ne[idx] = h[a]; // 插入,将该点下一个指向原来的头指向， 头再指向该点， 头插入 
	h[a] = idx ++ ; //头指向该点 
}

void dijkstra()
{
    memset(dist, 0x3f3f3f3f, sizeof dist);//距离初始化为无穷大
    dist[1] = 0;
    priority_queue<PII, vector<PII>, greater<PII> > heap;//小根堆
    heap.push({0, 1});//插入距离和节点编号

    while (heap.size())
    {
        PII t = heap.top();//取距离源点最近的点
        heap.pop();

        int ver = t.second, distance = t.first;//ver:节点编号，distance:源点距离ver的距离

        if (st[ver]) continue;//如果距离已经确定，则跳过该点
        st[ver] = true;

        for (int i = h[ver]; i != -1; i = ne[i])//更新ver所指向的节点距离
        {
            int j = e[i];
            if (dist[j] > dist[ver] + w[i])
            {
                dist[j] = dist[ver] + w[i];
                pr[j] = ver; // 储存路径指向前一个路径 
                heap.push({dist[j], j});//距离变小，则入堆， 排序 
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
        //add(b, a, c); 无向时
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
	}*/ //输出路径
    return 0;
}

