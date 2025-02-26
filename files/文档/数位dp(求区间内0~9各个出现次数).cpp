#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
using namespace std;

const int N = 2001;

ll f[N][N], k[N];
ll a, b;
ll dfs(bool lead, bool limit, int dp, int x, ll sum) {
    if(dp < 0) return sum;

    if(lead && !limit && f[dp][sum] != -1 ) return f[dp][sum];
    ll res = 0;
    ll up = limit ? k[dp] : 9;
    for (int i = 0; i <= up; i ++) {
        res += dfs((lead || i), (limit && i == up), dp - 1, x, sum + ((lead || i) && (i == x)));
    }
    if(lead && !limit) f[dp][sum] = res;
    return res;

}

ll handle(ll num, int x) {
    int p = 0;
    while (num) {
        k[p ++ ] = num % 10;
        num /= 10;
    }
    return dfs(0, 1, p - 1, x, 0);
}

int main()
{
     cin >> a >> b;

     for (int i = 0; i <= 9; i ++) {
         memset(f, -1, sizeof f);
         cout << handle(b, i) - handle(a - 1, i);
         if(i != 9) cout << ' ';
         else cout << endl;
      }
    

    return 0;
}


