#include<bits/stdc++.h>
using namespace std;

struct Good {
    int v, w;
};

int f[40100];

int main() {
    vector<Good> goods;
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i ++) {
        int v, w, s;
        cin >> v >> w >> s;
        for (int k = 1; k <= s; k *= 2) {
            s -= k;
            goods.push_back({ v * k, w * k });
        }
        if (s > 0) goods.push_back({ v * s, w * s });
    }

    for (auto good : goods) {
        for (int j = m; j >= good.w; j --) {
            f[j] = max(f[j], f[j - good.w] + good.v);
        }
    }
    cout << f[m] << endl;
    return 0;
}
