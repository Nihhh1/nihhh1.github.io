#include<iostream>
using namespace std;
int n,m;
const int N=1e5;
int q[N];
int main(){
    cin>>n>>m;
    for(int i=0;i<n;i++)cin>>q[i];
    while(m--){
    int x;
    cin>>x;
        int l=0,r=n-1;
        while(l<r){
            int mid=l+r>>1;
            if(q[mid]>=x)r=mid;
            else l=mid+1;
        }
        if(q[l]!=x)cout<<-1<<' ';
        else cout<<l<<' ';
        l=0,r=n-1;
        while(l<r){
            int mid=l+r+1>>1;
            if(q[mid]<=x)l=mid;
            else r=mid-1;
        }
        if(q[l]!=x)cout<<-1<<endl;
        else cout<<l<<endl;
    }
    return 0;
}
