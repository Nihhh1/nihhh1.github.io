#include<bits/stdc++.h>
using namespace std;
typedef pair<int,int>PII;
const int N=5010;
int p[N]; 
int find(int x){
	if(p[x]!=x)p[x]=find(p[x]);
	return p[x];
}
int main(){
	int n,m,q;
	while(~scanf("%d %d %d",&n,&m,&q)){
		for(int i=1;i<=n;i++)p[i]=i;
		while(m--){
			int a,b;
			scanf("%d %d",&a,&b);
			p[find(a)]=find(b);
		}
		while(q--){
			int a,b;
			scanf("%d %d",&a,&b);
			if(find(a)==find(b))puts("Yes");
			else puts("No");
		}
	}
	return 0;
}
