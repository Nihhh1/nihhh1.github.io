#include<bits/stdc++.h>
using namespace std;
 
template <typename T> inline void read(T& t) {
    int f = 0, c = getchar(); t = 0; 
    while (!isdigit(c)) f |= c == '-', c = getchar();
    while (isdigit(c)) t = t * 10 + c - 48, c = getchar();
    if (f) t = -t;
}

template <typename T> void print(T x) {
    if (x < 0) x = -x, putchar('-');
    if (x > 9) print(x / 10);
    putchar(x % 10 + 48);
} 

int main(){
	__int128 n;  
	read(n);
	print(n);
}
