#include<iostream>
using namespace std;
const int N=1e6+10;
int n;
int q[N];
int quick_sort(int l,int r,int k){
	if(l>=r) return q[l];
	int x=q[(l+r)>>1],i=l-1,j=r+1;
	while(i<j){
		while(q[++i]<x);
		while(q[--j]>x);
		if(i<j)swap(q[i],q[j]);
	}
	int sl=j-l+1;
	if(k<=sl)return quick_sort(l,j,k);
	return quick_sort(j+1,r,k-sl);
}
int main(){
	cin>>n;
	int k;
	cin>>k;
	for(int i=1;i<=n;i++)scanf("%d",&q[i]);
	cout<<quick_sort(1,n,k)<<endl;
}
