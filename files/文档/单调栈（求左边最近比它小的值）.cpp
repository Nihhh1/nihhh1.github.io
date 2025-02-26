#include<iostream>
using namespace std;
const int N = 1e5 + 10;
int n, a[N], tt ;
int main(){
    cin >> n;
    for(int i = 1; i <= n; i ++ ){
        int x;
        cin >> x;
        while(tt && a[tt] >= x) tt--;
        if(tt) cout << a[tt] << ' ';
        else cout << -1 << ' ';
        a[ ++ tt] = x;
    }
    return 0;
}
