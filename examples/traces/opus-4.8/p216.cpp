#include <bits/stdc++.h>
using namespace std;
int N, B, K = 0;
vector<int> adj[10005];
int prov[10005], parent[10005];
vector<int> caps, pendingLeftover;
vector<vector<int>> pending;
int main(){
    scanf("%d %d",&N,&B);
    for(int i=0;i<N-1;i++){ int a,b; scanf("%d %d",&a,&b); adj[a].push_back(b); adj[b].push_back(a);}
    pending.assign(N+1,{});
    vector<pair<int,int>> dfs; dfs.push_back({1,0}); parent[1]=0;
    while(!dfs.empty()){
        auto &top=dfs.back(); int u=top.first;
        if(top.second<(int)adj[u].size()){
            int v=adj[u][top.second++];
            if(v==parent[u]) continue;
            parent[v]=u; dfs.push_back({v,0});
        } else {
            vector<int> &pu=pending[u];
            pu.push_back(u);
            int p=parent[u];
            if((int)pu.size()>=B){ K++; caps.push_back(u); for(int x:pu) prov[x]=K; pu.clear(); }
            if(p!=0){ for(int x:pu) pending[p].push_back(x); }
            else pendingLeftover=pu;
            dfs.pop_back();
        }
    }
    if(K==0){ printf("0\n"); return 0; }
    for(int x: pendingLeftover) prov[x]=K;
    printf("%d\n", K);
    for(int i=1;i<=N;i++) printf("%d%c", prov[i], i==N?'\n':' ');
    for(int i=0;i<K;i++) printf("%d%c", caps[i], i==K-1?'\n':' ');
    return 0;
}
