#include<iostream>
using namespace std;
int main(){
	int l=0,r=n-1;
	while(l<r){
		int mid=(l+r)/2;
		if(a[mid]>x)r=mid;
		else l=mid+1;
	}//��߽� ��x��һ������x��ֵ
	while(l<r){
		int mid=(l+r+1)/2;
		if(a[mid]<x)l=mid;
		else r=mid-1;
	} //�ұ߽� ��x��һ��С��x��ֵ 
} 

// lower_bound(num,num+6,7)-num;    //���������е�һ�����ڻ���ڱ�������ֵ
// upper_bound(num,num+6,7)-num;    //���������е�һ�����ڱ�������ֵ 
