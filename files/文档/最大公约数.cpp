#include<iostream>
using namespace std;
int gcd(int a,int b){
	return b?gcd(b,a%b):a;
} 
long long int a, b, c;
int main(){
	cin >> a >> b >> c;
	int d1 = gcd(a, b);
	cout << d1;
	int d2 = gcd(a, c);
	int d3 = gcd(b, c);
	if((gcd(d1, d2)+ gcd(d2, d3) % 2 == 0))cout << "YES" << endl;
	else cout << "NO" << endl;

	
}
