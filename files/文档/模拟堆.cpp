#include<iostream>
using namespace std;
const int N = 1e5 + 10;
int h[N], hp[N], ph[N], n, size1;
void heap_swap(int a, int b){
	swap(ph[hp[a]], ph[hp[b]]);
	swap(hp[a], hp[b]);
	swap(h[a], h[b]);
}

void down(int u){
	int t = u;
	if(u * 2 <= size1 && h[u * 2] < h[t]) t = u * 2;
	if(u * 2 + 1 <= size1 && h[u * 2 + 1] < h[t])t = u * 2 + 1;
	if(u != t){
		heap_swap(u, t);
		down(t);
	}
}

void up(int u){
	while(u / 2 && h[u / 2] > h[u]){
		heap_swap(u / 2, u);
		u /= 2;
	}
}
int m;
int main(){
    cin >> n;
    while(n -- ){
        string op;
        cin >> op;
        if(op == "I"){//²åÈë 
            int a;
            cin >> a;
            size1 ++;
            m ++ ;
            ph[m] = size1;
            hp[size1] = m;
            h[size1] = a;
            up(size1);
        }
	else if(op == "DM"){
			heap_swap(1, size1);
			size1 -- ;
			down(1);
		}
       else if(op == "D"){
        	int a;
        	cin >> a;
        	a = ph[a];
        	heap_swap(a, size1);
        	size1 --;
        	down(a), up(a);
		}
		else if(op == "PM")cout << h[1] << endl;
		else if(op == "C"){
			int k, x;
			cin >> k >> x;
			k = ph[k];
			h[k] = x;
			down(k), up(k);
		}
    }
}
