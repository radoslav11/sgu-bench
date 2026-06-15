#include <bits/stdc++.h>
using namespace std;

struct Edge { int to; long long cap; };
vector<Edge> edges;
vector<vector<int>> g;
vector<int> it;

long long dfs(int u, int sink, long long f) {
    if (u == sink) return f;
    for (int &i = it[u]; i < (int)g[u].size(); i++) {
        int id = g[u][i];
        if (edges[id].cap > 0) {
            long long d = dfs(edges[id].to, sink, min(f, edges[id].cap));
            if (d > 0) { edges[id].cap -= d; return d; }
        }
    }
    return 0;
}

int main(){
    int N,M,L;
    scanf("%d %d %d",&N,&M,&L);
    vector<int> lvl(N+1);
    int src=-1, snk=-1;
    for(int i=1;i<=N;i++){scanf("%d",&lvl[i]); if(lvl[i]==1)src=i; if(lvl[i]==L)snk=i;}
    g.assign(N+1, {});
    vector<long long> origCap(M);
    vector<int> edgeId(M);
    for(int e=0;e<M;e++){
        int a,b; long long c; scanf("%d %d %lld",&a,&b,&c);
        origCap[e]=c;
        edgeId[e]=(int)edges.size();
        edges.push_back({b,c});
        g[a].push_back((int)edges.size()-1);
    }
    it.assign(N+1,0);
    while(dfs(src,snk,LLONG_MAX)>0){}
    for(int e=0;e<M;e++){
        printf("%lld\n", origCap[e]-edges[edgeId[e]].cap);
    }
    return 0;
}
