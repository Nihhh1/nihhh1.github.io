#include<bits/stdc++.h>
using namespace std;
const int N = 100003;
int n;
int e[N], h[N], ne[N], x, idx;
void add(int x){
    int k = (x % N + N) % N;
    e[idx] = x;
    ne[idx] = h[k];
    h[k] = idx ++ ;
}
int find(int x){
    int k = (x % N + N) % N;
    for(int i = h[k]; i != -1; i = ne[i] ){
        if(e[i] == x) return 1;
    }
    return 0;
}
int main(){
    ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);
    cin >> n;
    memset(h, -1, sizeof h);
    for(int i = 1; i <= n; i ++ ){
        char op;
        int x;
        cin >> op >> x;
        if(op == 'I') add(x);
        else {
            if((find(x))) cout << "Yes" << endl;
            else cout << "No" << endl;
        }
    }
    
}
