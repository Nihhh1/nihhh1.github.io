#include<bits/stdc++.h>
using namespace std;
const int N = 200003;
int n;
int  h[N], null = 0x3f3f3f3f;
int find(int x){
    
    int k = (x % N + N) % N;
    
    while(h[k] != null && h[k] != x){
        
        k ++ ;
        
        if(k == N) k = 0;
        
    }
    
    return k;
}
int main(){
    ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);
    cin >> n;
    
    memset(h, 0x3f3f3f3f, sizeof h);
    
    for(int i = 1; i <= n; i ++ ){
        char op;
        int x;
        cin >> op >> x;
        
        int k = find(x);
        
        if(op == 'I')  h[k] = x;
        
        else {
            
            if(h[k] != null) cout << "Yes" << endl;
            
            else cout << "No" << endl;
            
        }
        
    }
    return 0;
}
