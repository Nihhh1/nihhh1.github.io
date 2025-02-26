#include<iostream>
using namespace std;
const int N = 100010;
int a[N], q[N];
int n, len;
int main(){
    cin >> n;
    for(int i = 1; i <= n; i ++ )cin >> a[i];
    
    q[0] = -2e9;
    
    for(int i = 1; i <= n; i ++ ){
        int l = 0, r = len;
        while(l < r){
            int mid = l + r + 1 >> 1;
            if(q[mid] < a[i]) l = mid;
            else r = mid - 1;
        }
        len = max(len, r + 1);
        q[r + 1] = a[i]; // 替换但大小不变 
    }
    cout << len << endl;
}
