#include <bits/stdc++.h>
using namespace std;

int parent[400005];
int rnk[400005];

int find(int x){
    while(parent[x]!=x){
        parent[x]=parent[parent[x]];
        x=parent[x];
    }
    return x;
}

int main(){
    int m;
    scanf("%d",&m);
    
    map<pair<long long,long long>,int> id;
    int cnt=0;
    
    vector<pair<int,int>> edges(m);
    for(int i=0;i<m;i++){
        long long x1,y1,x2,y2;
        scanf("%lld %lld %lld %lld",&x1,&y1,&x2,&y2);
        auto p1=make_pair(x1,y1);
        auto p2=make_pair(x2,y2);
        int a,b;
        auto it=id.find(p1);
        if(it==id.end()){a=cnt;id[p1]=cnt++;}else a=it->second;
        it=id.find(p2);
        if(it==id.end()){b=cnt;id[p2]=cnt++;}else b=it->second;
        edges[i]=make_pair(a,b);
    }
    
    for(int i=0;i<cnt;i++){parent[i]=i;rnk[i]=0;}
    
    int ans=0;
    for(int i=0;i<m;i++){
        int a=find(edges[i].first);
        int b=find(edges[i].second);
        if(a==b){
            ans=i+1;
            break;
        }
        if(rnk[a]<rnk[b]) swap(a,b);
        parent[b]=a;
        if(rnk[a]==rnk[b]) rnk[a]++;
    }
    
    printf("%d\n",ans);
    return 0;
}
