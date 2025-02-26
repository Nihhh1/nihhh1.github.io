#include<bits/stdc++.h>
using namespace std;

const int N = 205;

int n,m,k;
int d[N][N];

void floyd()
{
    for(int k = 1; k <= n; k ++ )
        for(int i = 1; i <= n; i ++ )
            for(int j = 1; j <= n; j ++ )
                d[i][j] = min(d[i][j],d[i][k] + d[k][j]);
}

int main()
{
    cin>>n>>m>>k;

    for(int i = 1; i <= n; i ++ )
        for(int j = 1; j <= n; j ++ )
            if(i != j)
                d[i][j] = 0x3f3f3f3f;
            else
                d[i][j] = 0;//³õÊ¼»¯

    while(m -- )
    {
        int a,b,c;
        cin>>a>>b>>c;
        d[a][b] = min(d[a][b],c);
    }
    floyd();
    while(k -- )
    {
        int x,y;
        cin>>x>>y;
        if(d[x][y] >= 0x3f3f3f3f / 2)
            cout<<"impossible"<<endl;
        else
            cout<<d[x][y]<<endl;
    }
    return 0;
}
