#include <bits/stdc++.h>
#define fi first
#define se second
#define mp make_pair

// 欧拉定理 R+V-E=2 。
// R区域数 V顶点数 E边界数
// ans=E-V+1 
// V= n+C(n,4) E=C(n,2)+2C(n,4)
//ans =C(n,2)+C(n,4)-n+1
 


using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
const double eps = 1e-8;
const int NINF = 0xc0c0c0c0;
const int INF = 0x3f3f3f3f;
const ll mod  = 1e9 + 7;
const ll N = 1e6 + 5;

ll qpow(ll x,ll y){     // 逆元 
    ll ans=1;
    x%=mod;
    while(y>0){
        if(y&1) ans=ans*x%mod;
        x=x*x%mod;
        y>>=1;
    }
    return ans;
}

ll C(ll x,ll y){   //组合数 
    ll ans=1;
    for(ll i=x;i>x-y;i--){
        ans=ans*i%mod;
    }
    ll res=1;
    for(ll i=1;i<=y;i++){
        res=res*i%mod;
    }
    return ans*qpow(res,mod-2)%mod;
}

int main(){
    ll n;
    cin>>n;
    cout<<((C(n,2)+C(n,4)-n+1)%mod+mod)%mod<<'\n';
    return 0;
}
