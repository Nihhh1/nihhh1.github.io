#include<iostream>
using namespace std;
int main(){
	int l=0,r=n-1;
	while(l<r){
		int mid=(l+r)/2;
		if(a[mid]>x)r=mid;
		else l=mid+1;
	}//左边界 无x第一个大于x的值
	while(l<r){
		int mid=(l+r+1)/2;
		if(a[mid]<x)l=mid;
		else r=mid-1;
	} //右边界 无x第一个小于x的值 
} 

// lower_bound(num,num+6,7)-num;    //返回数组中第一个大于或等于被查数的值
// upper_bound(num,num+6,7)-num;    //返回数组中第一个大于被查数的值 
