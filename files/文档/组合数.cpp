/*#include<bits/stdc++.h>
using namespace std;
const int N = 2010, mod = 1e9 + 7;

int c[N][N]; 

void init(){
	for(int i = 0; i < N; i ++ )
		for(int j = 0; j <= i; j ++)
			if(!j) c[i][j] = 1;
			else c[i][j] = (c[i - 1][j] + c[i - 1][j - 1]) % mod;
}

int main(){
	init();
	int n;
	cin >> n;
	while(n -- ){
		int a, b;
		cin >> a >> b;
		cout << c[a][b] << endl;
	}
	return 0;
} 组合数 用公式递推预处理*/


/*#include<iostream>
#include<cstdio>
using namespace std;
typedef long long ll;
const int mod = 1000000007;
const int N = 100010;
ll f[N], inf[N];
ll qmi(int x, int k) {
    ll res = 1;

    while(k){
        if(k & 1)res = res * x % mod; 
        k >>= 1;
        x = 1ll * x * x % mod;
    }

    return res;
}

void init() {
    f[0] = inf[0] = 1;
    for(int i = 1; i < N; i ++ ) f[i] = f[i - 1] * i % mod;
    for(int i = 1; i < N; i ++ ) inf[i] = inf[i - 1] * qmi(i, mod - 2) % mod;

}
int main()
{
    init();
   int t;
   cin >> t;
   while(t -- ){
   int n, m;
   cin >> n >> m;
   ll res = f[n] * inf[m] % mod * inf[n - m] % mod;
   cout << res << endl;
   }
return 0;
} 用费马小定理求逆元预处理*/ 



