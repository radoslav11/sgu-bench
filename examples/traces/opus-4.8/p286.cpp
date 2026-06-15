#include <bits/stdc++.h>
using namespace std;

int main(){
    int N; long long K;
    scanf("%d %lld",&N,&K);
    long long M = N*K/2;
    vector<int> eu(M+1), ev(M+1);
    vector<vector<pair<int,int>>> adj(N+1); // (neighbor, edgeId)
    unordered_map<long long,int> emap;
    emap.reserve(M*2);
    for(int i=1;i<=M;i++){
        int u,v; scanf("%d %d",&u,&v);
        eu[i]=u; ev[i]=v;
        adj[u].push_back({v,i});
        adj[v].push_back({u,i});
        long long a=u,b=v; if(a>b)swap(a,b);
        emap[a*(long long)(N+1)+b]=i;
    }
    vector<char> usedEdge(M+1,false);
    vector<int> ptr(N+1,0);
    // orientation: dir[edge] = 1 if oriented eu->ev, 2 if ev->eu
    vector<int> dir(M+1,0);
    // Hierholzer per component
    for(int s=1;s<=N;s++){
        if(ptr[s]>= (int)adj[s].size()) continue;
        // check if has unused edge
        // run
        vector<int> stk;
        stk.push_back(s);
        vector<int> circuit;
        while(!stk.empty()){
            int v=stk.back();
            bool advanced=false;
            while(ptr[v] < (int)adj[v].size()){
                auto pr=adj[v][ptr[v]];
                if(usedEdge[pr.second]){ ptr[v]++; continue; }
                usedEdge[pr.second]=true;
                ptr[v]++;
                stk.push_back(pr.first);
                advanced=true;
                break;
            }
            if(!advanced){
                circuit.push_back(v);
                stk.pop_back();
            }
        }
        // circuit is euler circuit (reversed), consecutive vertices form edges
        for(size_t i=0;i+1<circuit.size();i++){
            int u=circuit[i], w=circuit[i+1];
            long long a=u,b=w; if(a>b)swap(a,b);
            auto it=emap.find(a*(long long)(N+1)+b);
            int eid=it->second;
            // orient u->w
            if(eu[eid]==u && ev[eid]==w) dir[eid]=1;
            else dir[eid]=2;
        }
    }
    // Build directed graph: edge u->v. Bipartite: left=out(u), right=in(v)
    // For each vertex u, outgoing edges. Match out(u) to in(v).
    // bipartite adjacency from left vertex u
    vector<vector<int>> badj(N+1); // for left u, list of edge ids (out edges of u)
    for(int i=1;i<=M;i++){
        if(dir[i]==1) badj[eu[i]].push_back(i); // u=eu, out
        else if(dir[i]==2) badj[ev[i]].push_back(i);
    }
    // helper to get target (in vertex) of edge given it's out of u
    auto target=[&](int eid)->int{
        if(dir[eid]==1) return ev[eid];
        else return eu[eid];
    };
    // Hopcroft-Karp
    // left nodes 1..N, right nodes 1..N
    vector<int> matchL(N+1,0), matchR(N+1,0); // matchL[u]=edge id, matchR[v]=u
    vector<int> distv(N+1);
    const int INF=1e9;
    auto bfs=[&]()->bool{
        queue<int>q;
        for(int u=1;u<=N;u++){
            if(matchL[u]==0){ distv[u]=0; q.push(u);}            
            else distv[u]=INF;
        }
        bool found=false;
        while(!q.empty()){
            int u=q.front();q.pop();
            for(int eid: badj[u]){
                int v=target(eid);
                int u2=matchR[v];
                if(u2==0){ found=true; }
                else if(distv[u2]==INF){
                    distv[u2]=distv[u]+1;
                    q.push(u2);
                }
            }
        }
        return found;
    };
    function<bool(int)> dfs=[&](int u)->bool{
        for(int eid: badj[u]){
            int v=target(eid);
            int u2=matchR[v];
            if(u2==0 || (distv[u2]==distv[u]+1 && dfs(u2))){
                matchL[u]=eid;
                matchR[v]=u;
                return true;
            }
        }
        distv[u]=INF;
        return false;
    };
    int matching=0;
    while(bfs()){
        for(int u=1;u<=N;u++){
            if(matchL[u]==0){
                if(dfs(u)) matching++;
            }
        }
    }
    if(matching<N){
        printf("NO\n");
        return 0;
    }
    printf("YES\n");
    for(int u=1;u<=N;u++){
        printf("%d\n", matchL[u]);
    }
    return 0;
}
