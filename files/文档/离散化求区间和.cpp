#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

const int N=300010;//坐标x的数量上限为1e5，两个坐标l,r的数量上限也为1e5,所以加起来为3*le5;
typedef pair<int,int> PII;//pair<int,int>类似于结构体的简写版，typedef定义了一个新的类型PII(跟结构体定义了一个结构体类型然后使用相似)

int a[N],s[N];

vector<int> alls;
vector<PII> add,query;
//使用二分查找x所在的位置，此时是alls(x,l,r)排好序的,返回的坐标也会是按照x的大小所给出的；
int find(int x)
{
    int l=0,r=alls.size()-1;
    while(l<r)
    {
        int mid=(l+r)/2;
        if(alls[mid]>=x) r=mid;
        else l=mid+1;
    }
    return r+1;//因为后续要使用前缀和，所以返回的坐标要加上1；
}

int main()
{
    int n,m;cin>>n>>m;
    //分别将要操作的四组数据记录在add和query中，将l，r，x的坐标值保存在alls中；
    for(int i=0;i<n;i++)
    {
        int x,c;
        cin>>x>>c;
        add.push_back({x,c});
        alls.push_back(x);
    }
    for(int i=0;i<m;i++)
    {
        int l,r;
        cin>>l>>r;
        query.push_back({l,r});
        alls.push_back(l);
        alls.push_back(r);
    }
    //将alls进行排序，并将重复的操作删除掉(如进行了两次在x的增值操作，应该去掉一个x保持平衡)；
    sort(alls.begin(),alls.end());
    alls.erase(unique(alls.begin(),alls.end()),alls.end());
    //一个迭代器从1开始直到末尾结束，itdm.first是x，second是r(在上方循环中可知)；
    for(auto itdm : add)
    {
        int x;
        x=find(itdm.first);
        a[x]+=itdm.second;
    }
    //只循环x是因为x的坐标加上了值，而l，r可能有(l或r与x的值相同)，且定义全局变量数组，其余值默认为0，故可以方便计算；
    for(int i=1;i<=alls.size();i++) s[i]=s[i-1]+a[i];

    for(auto itdm : query)
    {
        int l,r;
        l=find(itdm.first);//找出l，r在a中的坐标
        r=find(itdm.second);
        printf("%d\n",s[r]-s[l-1]);//前缀和上方已计算，所以可直接输出，记得加上换行符！
    }
    return 0;
}
