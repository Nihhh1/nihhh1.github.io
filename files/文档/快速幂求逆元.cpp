#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int mod = 9973; 
int qmi(int x, int k, int mod) {
	int res = 1;
	while (k) {
		if(k & 1) res = (ll) res * x % mod;
		x = (ll) x * x % mod;
		k >>= 1;
	}
	return res;
}

int main() {
	int t;
	cin >> t;
	while(t -- ){
		int a, b;
		cin >> a >> b;
		int res = qmi(b, mod - 2, mod);
		res = a * res % mod;
		cout << res << endl;
	} 
	return 0;
}
