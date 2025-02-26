#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

const int N=300010;//����x����������Ϊ1e5����������l,r����������ҲΪ1e5,���Լ�����Ϊ3*le5;
typedef pair<int,int> PII;//pair<int,int>�����ڽṹ��ļ�д�棬typedef������һ���µ�����PII(���ṹ�嶨����һ���ṹ������Ȼ��ʹ������)

int a[N],s[N];

vector<int> alls;
vector<PII> add,query;
//ʹ�ö��ֲ���x���ڵ�λ�ã���ʱ��alls(x,l,r)�ź����,���ص�����Ҳ���ǰ���x�Ĵ�С�������ģ�
int find(int x)
{
    int l=0,r=alls.size()-1;
    while(l<r)
    {
        int mid=(l+r)/2;
        if(alls[mid]>=x) r=mid;
        else l=mid+1;
    }
    return r+1;//��Ϊ����Ҫʹ��ǰ׺�ͣ����Է��ص�����Ҫ����1��
}

int main()
{
    int n,m;cin>>n>>m;
    //�ֱ�Ҫ�������������ݼ�¼��add��query�У���l��r��x������ֵ������alls�У�
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
    //��alls�������򣬲����ظ��Ĳ���ɾ����(�������������x����ֵ������Ӧ��ȥ��һ��x����ƽ��)��
    sort(alls.begin(),alls.end());
    alls.erase(unique(alls.begin(),alls.end()),alls.end());
    //һ����������1��ʼֱ��ĩβ������itdm.first��x��second��r(���Ϸ�ѭ���п�֪)��
    for(auto itdm : add)
    {
        int x;
        x=find(itdm.first);
        a[x]+=itdm.second;
    }
    //ֻѭ��x����Ϊx�����������ֵ����l��r������(l��r��x��ֵ��ͬ)���Ҷ���ȫ�ֱ������飬����ֵĬ��Ϊ0���ʿ��Է�����㣻
    for(int i=1;i<=alls.size();i++) s[i]=s[i-1]+a[i];

    for(auto itdm : query)
    {
        int l,r;
        l=find(itdm.first);//�ҳ�l��r��a�е�����
        r=find(itdm.second);
        printf("%d\n",s[r]-s[l-1]);//ǰ׺���Ϸ��Ѽ��㣬���Կ�ֱ��������ǵü��ϻ��з���
    }
    return 0;
}
