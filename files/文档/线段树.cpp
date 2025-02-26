#include<iostream>
#include<algorithm>
using namespace std;
const int N = 2e5 + 10;
const int M = 1e6 + 10;
int arr[N], tree[M];

void build_tree(int start, int end, int node){
	if(start == end){
		tree[node] = arr[start];
		return ;
	}
	int mid = start + end >> 1;
	int left_node = 2 * node + 1;
	int right_node  = 2 * node + 2;
	build_tree(start, mid, left_node);
	build_tree(mid + 1, end, right_node);
	tree[node] = max(tree[left_node], tree[right_node]);
}

void updata(int idx, int val, int start, int end, int node){
	if(start == end){
		arr[idx] = val;
		tree[node] = val;
		return ;
	}
	int left_node = 2 * node + 1;
	int right_node = 2 * node + 2;
	int mid = start + end >> 1;
	if(idx <= mid) updata(idx, val, start, mid, left_node);
	else updata(idx, val, mid + 1, end, right_node);
	tree[node] = max(tree[left_node], tree[right_node]);
}

int query(int L, int R, int start, int end, int node){
	if(end < L || start > R){
		return 0;
	}
	else if(start >= L && end <= R){
		return tree[node];
	}
	int mid = start + end >> 1;
	int left_node = 2 * node + 1;
	int right_node = 2 * node + 2;
	int max_left = query(L, R, start, mid, left_node);
	int max_right = query(L, R, mid + 1, end, right_node);
	return max(max_left, max_right);
}
int n, m;
int main(){
	ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
	while(cin >> n >> m){
	for(int i = 1; i <= n; i ++) cin >> arr[i];
	build_tree(1, n, 0);
	while(m -- ){
		char q;
		int a, b;
		cin >> q >> a >> b;
		if(q == 'U'){
			updata(a, b, 1, n, 0);
		}
		else cout << query(a, b, 1, n, 0) << endl;
	}
}
	return 0;
}
