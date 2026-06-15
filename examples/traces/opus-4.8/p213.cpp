#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,m,s,t;
    scanf("%d %d %d %d",&n,&m,&s,&t);
    vector<vector<pair<int,int>>> adj(n+1);
    vector<int> eu(m+1),ev(m+1);
    for(int i=1;i<=m;i++){
        int a,b;scanf("%d %d",&a,&b);
        eu[i]=a;ev[i]=b;
        adj[a].push_back({b,i});
        adj[b].push_back({a,i});
    }
    vector<int> dist(n+1,-1);
    queue<int>q;
    dist[s]=0;q.push(s);
    while(!q.empty()){
        int u=q.front();q.pop();
        for(auto&pr:adj[u]){
            int v=pr.first;
            if(dist[v]==-1){dist[v]=dist[u]+1;q.push(v);}
        }
    }
    int L=dist[t];
    vector<vector<int>> cuts(L);
    for(int i=1;i<=m;i++){
        int a=dist[eu[i]],b=dist[ev[i]];
        if(a<0||b<0) continue;
        if(abs(a-b)==1){
            int lo=min(a,b);
            if(lo>=0&&lo<L) cuts[lo].push_back(i);
        }
    }
    printf("%d\n",L);
    for(int i=0;i<L;i++){
        printf("%d",(int)cuts[i].size());
        for(int e:cuts[i]) printf(" %d",e);
        printf("\n");
    }
    return 0;
}
