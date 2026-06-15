#include<bits/stdc++.h>
using namespace std;
int main(){
    int n,m;scanf("%d%d",&n,&m);
    vector<tuple<int,int,long long>>edges(m);
    vector<vector<pair<int,long long>>>inc(n+1);
    for(int i=0;i<m;i++){int u,v;long long w;scanf("%d%d%lld",&u,&v,&w);edges[i]={u,v,w};inc[v].push_back({u,w});}
    vector<int>alive(n+1,0);
    // BFS/propagation: alive if no incoming OR can always get tokens
    // Try: alive[v] = 1 if for all incoming arcs (u,v,w): alive[u] and w>0? No...
    // Actually vertex 6: incoming arc (1,6,0) w=0, but alive[6]=1
    // So tokens matter dynamically
    // Different approach needed
    for(int i=1;i<=n;i++)printf("%d\n",alive[i]);
}
