#include<bits/stdc++.h>
using namespace std;
const int N = 6010;
int f[N][2], e[N], h[N], happy[N], nex[N], idx;
int has_father[N];

void add(int a, int b){
    e[idx] = b;
    nex[idx] = h[a];
    h[a] = idx ++ ;
}

void dfs(int u){
    f[u][1] = happy[u];
    
    for(int i = h[u]; i != -1; i = nex[i]){
        int j = e[i];
        
        dfs(j);
        
        f[u][1] += f[j][0];
        f[u][0] += max(f[j][1], f[j][0]);
    }
    
}

int main(){
    int n;
    cin >> n;
    memset(h, - 1, sizeof h);
    for(int i = 1; i <= n; i ++ ) cin >> happy[i];
    
    for(int i = 1; i < n; i ++ ){
        int a, b;
        cin >> a >> b;
        add(b, a);
        has_father[a] = 1;
    }
    
    int k = 1;
    while(has_father[k]) k ++ ;
    
    dfs(k);
    
    cout << max(f[k][0], f[k][1]);
    return 0;
}
