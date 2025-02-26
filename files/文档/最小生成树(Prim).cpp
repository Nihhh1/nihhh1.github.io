#include<bits/stdc++.h>
using namespace std;
const int N=5010;
int n,m;
int g[N][N];
int dist[N];
bool v[N];
int Prim(){
	memset(dist,0x3f,sizeof dist);
	dist[1]=0;
	int res=0;
	for(int i=0;i<n;i++){
		int t=-1;
		for(int j=1;j<=n;j++){
			if(!v[j]&&(t==-1||dist[t]>dist[j]))t=j;
		}
		if(dist[t]==0x3f3f3f3f)return 0x3f3f3f3f;
		res+=dist[t];
		v[t]=true;
		for(int j=1;j<=n;j++){
			dist[j]=min(dist[j],g[t][j]);
		}
	}
	return res;
}
int main(){
	cin>>n>>m;
	int a,b,c;
	memset(g,0x3f,sizeof(g));
	while(m--){
		scanf("%d%d%d",&a,&b,&c);
		g[a][b]=g[b][a]=min(g[a][b],c);
	}//初始化领接矩阵 
	int t=Prim();
	if(t==0x3f3f3f3f)puts("orz");
	else cout<<t<<endl;
	return 0;
}
