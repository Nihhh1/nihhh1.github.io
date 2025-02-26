#include<bits/stdc++.h>
using namespace std;
int n,m;
struct node{
	int data;
	node* next ;
	node* befor;
}; 
node* build(node* head){
	node* p=head, * q;
	p->befor =NULL;
	for(int i=1;i<=10;i++){
		q=new node;
		scanf("%d",&q->data);
		p->next = q;
		q->befor =p;
		p = q;
	}
	p->next =NULL; 
	return p;
}
/*void del(node *head,int x){
	for(int i=1;i<x;i++){
		if(head->next !=NULL)
		head=head->next;
		else return;
	}
	if(head->next==NULL)return;
	head->next=head->next->next;
} É¾*/

/*void add(node *head,int a,int b){
	for(int i=1;i<=a;i++){
		if(head->next !=NULL)
		head=head->next;
		else return;
	}
	node *p=new node;
	p->data =b;
	p->next =head->next ;
	head->next =p;
}//Ôö */
void show (node* head){
	while(head->befor!=NULL){
		printf("%d ", head->data );
		head=head->befor;
		if(head->befor ==NULL)printf("\n");
	}
}
int main(){
	node *head;
	head=new node;
	node* tail=build(head);
	show(tail);
	return 0;
}
