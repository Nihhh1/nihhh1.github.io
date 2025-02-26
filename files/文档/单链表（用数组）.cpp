#include<iostream>
using namespace std;
const int N = 1e6 + 10;
int e[N], nex[N], m, k, idx, head;
void init(){
	head = -1;
	idx = 0;
}

void add_to_head(int x){
	e[idx] = x, nex[idx] = head, head = idx, idx ++ ;
}

void add(int k, int x){
	e[idx] = x;
	nex[idx] = nex[k];
	nex[k] = idx;
	idx ++ ;
}

void remove(int k){
	nex[k] = nex[nex[k]];
}
int main(){
	cin >> m;
	init();
	while(m -- ){
		char op;
		int x, k;
		cin >> op;
		if(op == 'H'){
			cin >> x;
			add_to_head(x);
		}
		if(op == 'I'){
			cin >> k >> x;
			add(k - 1, x);
		}
		if(op == 'D'){
			cin >> k;
			if (k == 0) head = nex[head];
			remove(k - 1);
		}
	}
	for(int i = head; i != -1; i = nex[i]) cout << e[i] << ' ';
	cout << endl ;
	return 0;
}
