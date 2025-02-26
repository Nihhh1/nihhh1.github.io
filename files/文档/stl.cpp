#include<iostream>
#include<algorithm>
using namespace std;
const int N=2e5+10;
int a[N];
int n,q,res,k;
int main() {
 	cin>>n>>q;
 	for(int i=1;i<=n;i++)scanf("%d",&a[i]);
 	sort(a+1,a+1+n);
 	int k;
 	while(q--){
 		res=0;
 		cin>>k;
 		int l=1,r=n;
		while(l<r){
		int mid=(l+r)/2;
		if(a[mid]>=k)r=mid;
		else l=mid+1;
		}
		if(a[r]<k&&r==n)res=0;
		else res=n-r+1;
		 cout<<res<<endl;
	 }
	return 0;
}
