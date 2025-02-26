void dfs(int step)    //步长
{
    if(/*跳出循环的条件*/){
        return;    //return十分关键，否则循环将会无法跳出
    }
    /*函数主体
    对功能进行实现*/
    for(/*对现有条件进行罗列*/){
        if(/*判断是否合理*/){
            //将条件修改
            dfs(/*新的step*/)
            /*！重中之重，当跳出那层循环后将数据全部归位*/
        }    
    }
}

#include<iostream>
using namespace std;
int a[10];
int v[10];
int n;
void dfs(int x){
    if(x == n ){
        for(int i = 0; i <= n - 1; i ++ )cout << a[i] << ' ';
        puts("");
        return ;
    }
    for(int i = 1; i <= n; i ++ ){
        if(!v[i]){
            v[i] = 1;
            a[x] = i;
            dfs(x + 1);
            v[i] = 0;
        }
    }
}
int main(){
    cin >> n;
    dfs(0);
    return 0;
}

