#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, M;
    scanf("%d %d", &N, &M);
    vector<array<int,4>> edges(M);
    vector<vector<pair<int,int>>> adj(N+1); // for dijkstra: (to, weight)
    vector<vector<pair<int,int>>> tadj(N+1); // tree adjacency: (to, weight)
    for(int i=0;i<M;i++){
        int a,b,l,t; scanf("%d %d %d %d",&a,&b,&l,&t);
        edges[i]={a,b,l,t};
        adj[a].push_back({b,l});
        adj[b].push_back({a,l});
        if(t==1){
            tadj[a].push_back({b,l});
            tadj[b].push_back({a,l});
        }
    }
    const long long INF = (long long)4e18;
    vector<long long> d(N+1, INF);
    // Dijkstra from 1
    {
        priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
        d[1]=0; pq.push({0,1});
        while(!pq.empty()){
            auto [dist,u]=pq.top(); pq.pop();
            if(dist>d[u]) continue;
            for(auto [v,w]:adj[u]){
                if(d[u]+w<d[v]){
                    d[v]=d[u]+w;
                    pq.push({d[v],v});
                }
            }
        }
    }
    // build tree via BFS on tree edges from 1
    int LOG=1;
    while((1<<LOG)<N) LOG++;
    LOG++;
    vector<int> par(N+1,0), depth(N+1,0);
    vector<vector<int>> up(LOG, vector<int>(N+1,0));
    vector<char> vis(N+1,0);
    {
        // BFS
        queue<int> q;
        q.push(1); vis[1]=1; par[1]=0; depth[1]=0;
        while(!q.empty()){
            int u=q.front(); q.pop();
            for(auto [v,w]:tadj[u]){
                if(!vis[v]){
                    vis[v]=1;
                    par[v]=u;
                    depth[v]=depth[u]+1;
                    q.push(v);
                }
            }
        }
    }
    // binary lifting
    for(int v=1;v<=N;v++) up[0][v]=par[v];
    for(int k=1;k<LOG;k++){
        for(int v=1;v<=N;v++){
            up[k][v]=up[k-1][ up[k-1][v] ];
        }
    }
    auto lca=[&](int a,int b)->int{
        if(depth[a]<depth[b]) swap(a,b);
        int diff=depth[a]-depth[b];
        for(int k=0;k<LOG;k++) if(diff&(1<<k)) a=up[k][a];
        if(a==b) return a;
        for(int k=LOG-1;k>=0;k--){
            if(up[k][a]!=up[k][b]){ a=up[k][a]; b=up[k][b]; }
        }
        return par[a];
    };
    
    // candidate edges: t=0 with both finite d
    // val = d[u]+d[v]+w
    vector<array<long long,3>> cand; // val, u, v
    for(int i=0;i<M;i++){
        int a=edges[i][0], b=edges[i][1], l=edges[i][2], t=edges[i][3];
        if(t==0){
            if(d[a]<INF && d[b]<INF && vis[a] && vis[b]){
                long long val = d[a]+d[b]+(long long)l;
                cand.push_back({val,(long long)a,(long long)b});
            }
        }
    }
    sort(cand.begin(), cand.end(), [](const array<long long,3>&x, const array<long long,3>&y){
        return x[0]<y[0];
    });
    
    // union find to jump over covered vertices
    vector<int> uf(N+1);
    for(int i=0;i<=N;i++) uf[i]=i;
    function<int(int)> findf=[&](int x)->int{
        while(uf[x]!=x){ uf[x]=uf[uf[x]]; x=uf[x]; }
        return x;
    };
    
    vector<long long> ans(N+1, INF);
    
    for(auto &c:cand){
        long long val=c[0];
        int u=(int)c[1], v=(int)c[2];
        int L=lca(u,v);
        int dl=depth[L];
        // cover from u up to L (exclusive)
        int cur=u;
        while(true){
            cur=findf(cur);
            if(depth[cur]<=dl) break;
            if(val - d[cur] < ans[cur]) ans[cur]=val - d[cur];
            uf[cur]=par[cur];
        }
        cur=v;
        while(true){
            cur=findf(cur);
            if(depth[cur]<=dl) break;
            if(val - d[cur] < ans[cur]) ans[cur]=val - d[cur];
            uf[cur]=par[cur];
        }
    }
    
    // output for cities 2..N
    string out;
    for(int i=2;i<=N;i++){
        if(i>2) out+=' ';
        if(!vis[i] || d[i]==INF){
            out+="-1";
        } else if(ans[i]>=INF){
            out+="-1";
        } else {
            out+=to_string(ans[i]);
        }
    }
    out+='\n';
    fputs(out.c_str(), stdout);
    return 0;
}
