void dfs(int step)    //����
{
    if(/*����ѭ��������*/){
        return;    //returnʮ�ֹؼ�������ѭ�������޷�����
    }
    /*��������
    �Թ��ܽ���ʵ��*/
    for(/*������������������*/){
        if(/*�ж��Ƿ����*/){
            //�������޸�
            dfs(/*�µ�step*/)
            /*������֮�أ��������ǲ�ѭ��������ȫ����λ*/
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

