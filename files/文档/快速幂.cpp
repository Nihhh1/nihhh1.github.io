#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
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
	int x, k, mod;
	cin >> x >> k >> mod;
	cout << qmi(x, k, mod) << endl; 
	return 0;
}
