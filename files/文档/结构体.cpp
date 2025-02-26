#include<stdio.h>
struct student{
	int id;
	int ch;
	int ma;
	int en;
}s[100],p;
	int main(){
		int n;
		scanf("%d",&n);
		for(int i=0;i<n;i++){
			scanf("%d%d%d%d",&s[i].id,&s[i].ch,&s[i].ma,&s[i].en);
		}
		for(int i=0;i<n;i++){
			for(int j=0;j<n-i-1;j++){
				if(s[j].id>s[j+1].id){
					p=s[j];
					s[j]=s[j+1];
					s[j+1]=p;
				}
			}
		}
		int t,max,k=0;
		scanf("%d",&t);
		if(t==1){
			max=s[0].ch;
			for(int i=0;i<n;i++){
				if(s[i].ch>max){
					max=s[i].ch;
					k=i;
				}
			}
			for(int i=1;i<n;i++){
				if(max==s[i].ch)
				printf("%d %d %d %d\n",s[i].id,s[i].ch,s[i].ma,s[i].en);
			}
		}
				if(t==2){
			max=s[0].ma;
			for(int i=1;i<n;i++){
				if(s[i].ma>max){
					max=s[i].ma;
					k=i;
				}
			}
			for(int i=0;i<n;i++){
				if(max==s[i].ma)
				printf("%d %d %d %d\n",s[i].id,s[i].ch,s[i].ma,s[i].en);
			}
		}
				if(t==3){
			max=s[0].en;
			for(int i=0;i<n;i++){
				if(s[i].en>max){
					max=s[i].en;
					k=i;
				}
			}
			for(int i=1;i<n;i++){
				if(max==s[i].en)
				printf("%d %d %d %d\n",s[i].id,s[i].ch,s[i].ma,s[i].en);
			}
		}
		
	return 0;
}
