#include <bits/stdc++.h>
using namespace std;
#define IOS ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);
#define ll long long
//#define int ll
#define debug cout<<"fuck"<<endl;
#define pb  push_back
#define endl '\n'
#define fi first
#define se second
#define db double
#define pii pair<int,int>
#define mp make_pair
#define mian main
#define whiel while
#define retrun return
const int mod=(int)1e9+7;
int add(int a, int b) {if((a += b) >= mod) a -= mod; return a < 0 ? a + mod : a;}
int mul(int a, int b) {return 1ll * a * b % mod;}
const int maxn=(int)1e3+5;
int n,k;
string s;
bool dp[maxn][maxn];
int okk=0;
void dfs(int pos,int lef)
{
    if(pos==n)
    {
        if(lef==0)okk=1;
        return;
    }
    if(s[pos]!='?')
    {
        int val=s[pos]-'0';
        if(!dp[pos][(lef*10+val)%k])
        {
            dfs(pos+1,(lef*10+val)%k);
            if (okk)return;
            dp[pos][(lef*10+val)%k]=1;
        }
    }
    else
    {
        int i;
        i=((pos==0)?1:0);//保证没有前导0
        for(;i<10;i++)
        {
            if(!dp[pos][(lef*10+i)%k])
            {
                s[pos] = '0' + i;
                dfs(pos + 1, (lef * 10 + i) % k);
                if (okk)return;
                s[pos] = '?';
                dp[pos][(lef * 10 + i) % k] = 1;
            }
        }
    }
}
int main()
{
    IOS
    cin>>s>>k;
    n=s.size();
    dfs(0,0);
    //debug
    if(okk)cout<<s<<endl;
    else cout<<'*'<<endl;
    retrun 0;
}
