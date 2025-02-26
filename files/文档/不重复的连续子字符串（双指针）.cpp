#include<iostream>
using namespace std;
int n;
const int N = 1e5 + 10;
int a[N], s[N];
int main(){
    cin >> n;
    int res = 0;
    for(int i = 1; i <= n; i ++ )cin >> a[i];
    for(int j = 1, i = 1; i <= n; i ++ ){
        s[a[i]] ++ ;
        while(s[a[i]] > 1){
            s[a[j]] -- ;
            j ++ ;
        }
        res = max(res, i - j + 1);
    }
    cout << res << endl;
    return 0;
}
