#include<iostream>
#include<vector>
using namespace std;
vector<int> mul(vector<int> &A, int b){
    vector<int> C;
    int t=0;
    for(int i = 0; i < A.size() || t; i++){
        if(i < A.size()) t += A[i] * b;
        C.push_back(t % 10);
        t = t / 10;
    }
    return C;
}

int main(){
    string a;
    int b;
    vector<int> A;
    cin >> a>> b;
    if(b == 0){
        cout<< 0 << endl;
        return 0;
    }
    for(int i = a.size() - 1; i >= 0 ;i--) A.push_back(a[i] - '0');
    vector<int> C = mul(A,b);
    for(int i = C.size() - 1; i >= 0 ;i--) cout<<C[i];
}
