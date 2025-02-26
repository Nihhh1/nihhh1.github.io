#include<bits/stdc++.h>
using namespace std;
int v[22][22];
char a[22][22];
struct point{
	int x;
	int y;
	int step;
};
queue<point> r;
int dx[4]={0,1,0,-1};
int dy[4]={1,0,-1,0};
int main(){
	int n,m,sx,sy,p,q;
	scanf("%d%d",&m,&n);
	getchar();
	for(int i=1;i<=m;i++){
		for(int j=1;j<=n;j++){
		cin>>a[i][j];
		}
	}
	for(int i=1;i<=m;i++){
		for(int j=1;j<=n;j++){
			if(a[i][j]=='@'){
				sx=j;
				sy=i;
				break; 
			}
		}
	}
	
	point start;
	start.x=sx;
	start.y=sy;
	start.step=0;
	r.push(start);
	v[sy][sx]=1;
	int flag=0;
	
	while(!r.empty()){
		int x=r.front().x;
		int y=r.front().y;
		for(int i=0;i<4;i++){
			int tx,ty;
			tx=x+dx[i];
			ty=y+dy[i];
			if(a[ty][tx]=='*'){
			flag=1;
			printf("%d\n",r.front().step+1);
			break;
			}
			if(tx>=1&&tx<=n&&ty>=1&&ty<=m&&a[ty][tx]=='.'&&v[ty][tx]==0){
				point temp;
				temp.x=tx;
				temp.y=ty;
				temp.step=r.front().step+1;
				
				r.push(temp);
				v[ty][tx]=1;
			}
		}
		if(flag==1)break;
		r.pop();
	}
	
	if(flag==0)
		printf("-1\n");
	return 0;
}
