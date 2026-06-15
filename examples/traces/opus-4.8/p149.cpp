#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    if(scanf("%d",&n)!=1) return 0;
    vector<vector<pair<int,long long>>> adj(n+1);
    for(int i=2;i<=n;i++){
        int p; long long w;
        scanf("%d %lld",&p,&w);
        adj[i].push_back({p,w});
        adj[p].push_back({i,w});
    }
    
    if(n==1){
        printf("0\n");
        return 0;
    }
    
    // down1[v] = longest downward path, down2[v] = second longest (different child)
    // we'll root at 1, do iterative DFS
    vector<long long> down1(n+1,0), down2(n+1,0), up(n+1,0);
    vector<int> down1child(n+1,-1);
    vector<int> parent(n+1,0);
    vector<long long> pw(n+1,0); // weight to parent
    
    // BFS order for processing
    vector<int> order;
    order.reserve(n);
    vector<bool> vis(n+1,false);
    {
        // iterative DFS to get order and parent
        vector<int> stk;
        stk.push_back(1);
        vis[1]=true;
        while(!stk.empty()){
            int u=stk.back(); stk.pop_back();
            order.push_back(u);
            for(auto&e:adj[u]){
                int v=e.first; long long w=e.second;
                if(!vis[v]){
                    vis[v]=true;
                    parent[v]=u;
                    pw[v]=w;
                    stk.push_back(v);
                }
            }
        }
    }
    
    // process in reverse order to compute down1, down2
    for(int i=n-1;i>=0;i--){
        int u=order[i];
        for(auto&e:adj[u]){
            int v=e.first; long long w=e.second;
            if(v==parent[u]) continue;
            long long cand = down1[v]+w;
            if(cand>down1[u]){
                down2[u]=down1[u];
                down1[u]=cand;
                down1child[u]=v;
            } else if(cand>down2[u]){
                down2[u]=cand;
            }
        }
    }
    
    // process in order to compute up
    up[1]=0;
    for(int i=0;i<n;i++){
        int u=order[i];
        for(auto&e:adj[u]){
            int v=e.first; long long w=e.second;
            if(v==parent[u]) continue;
            // up[v] = w + max(up[u], best down of u not going through v)
            long long bestDown;
            if(down1child[u]==v) bestDown=down2[u];
            else bestDown=down1[u];
            up[v]=w+max(up[u],bestDown);
        }
    }
    
    for(int i=1;i<=n;i++){
        printf("%lld\n", max(down1[i],up[i]));
    }
    
    return 0;
}
