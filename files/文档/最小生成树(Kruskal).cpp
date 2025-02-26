#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
const int N = 100010;
int p[N];//���沢�鼯

struct E{
    int a;
    int b;
    int w;
    bool operator < (const E& rhs){//ͨ���߳���������
        return this->w < rhs.w;
    }

}edg[N * 2];
int res = 0;

int n, m;
int cnt = 0;
int find(int a){//���鼯������
    if(p[a] != a) p[a] = find(p[a]);
    return p[a];
}
void klskr(){
    for(int i = 1; i <= m; i++)//���γ��Լ���ÿ����
    {
        int pa = find(edg[i].a)��// a �����ڵļ���
        int pb = find(edg[i].b);// b �����ڵļ���
        if(pa != pb){//��� a b ����һ��������
            res += edg[i].w;//a b ֮��������Ҫ
            p[pa] = pb;// �ϲ�a b
            cnt ++; // �����ı�����+1
        }
    }
}
int main()
{

    cin >> n >> m;
    for(int i = 1; i <= n; i++) p[i] = i;//��ʼ�����鼯
    for(int i = 1; i <= m; i++){//����ÿ����
        int a, b , c;
        cin >> a >> b >>c;
        edg[i] = {a, b, c};
    }
    sort(edg + 1, edg + m + 1);//���߳�����
    klskr();
    if(cnt < n - 1) {//��������ı�С�ڵ���-1��������ͨ
        cout<< "impossible";
        return 0;
    }
    cout << res;
    return 0;
}
