#include<iostream>
using namespace std;
const int N = 1e5 + 10;
int p[N], n, m, size1[N];
int find(int x){
    if(p[x] != x)  p[x] = find(p[x]);
    return p[x];
}
int main(){
    cin >> n >> m;
    for(int i = 1; i <= n; i ++ ){
        p[i] = i;
        size1[i] = 1;
    }
    while(m -- ){
        char q[5];
        int a, b;
        scanf("%s", q);
        if(q[0] == 'C'){
            cin >> a >> b;
            if(find(a) == find(b)) continue;
            size1[find(b)] += size1[find(a)] ;
            p[find(a)] = b;
        }
        if(q[0] == 'Q' && q[1] == '1'){
            cin >> a >> b;
            if(find(a) == find(b)) cout << "Yes" << endl;
            else cout << "No" << endl;
        }
        if(q[0] == 'Q' && q[1] == '2'){
            cin >> a;
            cout << size1[find(a)] << endl;
        }
    }
    return 0;
}
