#include <bits/stdc++.h>
using namespace std;

struct Dinic {
    int n;
    vector<int> to, cap, head, nxt;
    vector<int> level, it;
    Dinic(int n): n(n), head(n,-1), level(n), it(n) {}
    int addEdge(int u,int v,int c){
        int id=to.size();
        to.push_back(v); cap.push_back(c); nxt.push_back(head[u]); head[u]=id;
        to.push_back(u); cap.push_back(0); nxt.push_back(head[v]); head[v]=id+1;
        return id;
    }
    bool bfs(int s,int t){
        fill(level.begin(),level.end(),-1);
        queue<int>q; level[s]=0; q.push(s);
        while(!q.empty()){
            int u=q.front();q.pop();
            for(int e=head[u];e!=-1;e=nxt[e]){
                if(cap[e]>0 && level[to[e]]<0){
                    level[to[e]]=level[u]+1;
                    q.push(to[e]);
                }
            }
        }
        return level[t]>=0;
    }
    int dfs(int u,int t,int f){
        if(u==t)return f;
        for(int &e=it[u];e!=-1;e=nxt[e]){
            if(cap[e]>0 && level[to[e]]==level[u]+1){
                int d=dfs(to[e],t,min(f,cap[e]));
                if(d>0){cap[e]-=d;cap[e^1]+=d;return d;}
            }
        }
        return 0;
    }
    int maxflow(int s,int t){
        int flow=0;
        while(bfs(s,t)){
            for(int i=0;i<n;i++)it[i]=head[i];
            int f;
            while((f=dfs(s,t,INT_MAX))>0)flow+=f;
        }
        return flow;
    }
};

int main(){
    int N,K;
    if(!(cin>>N>>K)) return 0;
    // nodes
    int S=0;
    // students 1..N
    // universities N+1..N+K
    int T=N+K+1;
    int SS=N+K+2;
    int TT=N+K+3;
    int total=N+K+4;
    Dinic din(total);
    
    // S->student
    for(int i=1;i<=N;i++) din.addEdge(S,i,1);
    
    // store student->uni edge ids
    vector<array<int,3>> studEdges; // student, uni, edgeid
    
    for(int i=1;i<=N;i++){
        int m; cin>>m;
        for(int j=0;j<m;j++){
            int u; cin>>u;
            if(u>=1 && u<=K){
                int id=din.addEdge(i, N+u, 1);
                studEdges.push_back({i,u,id});
            }
        }
    }
    
    // d[] adjustment for lower bounds on uni->T (lower 2, upper N)
    // d[T]+=2 per uni, d[uni]-=2
    // uni->T edge with cap (N-2) if N>=2 else 0
    for(int u=1;u<=K;u++){
        int cup = N-2;
        if(cup<0) cup=0;
        din.addEdge(N+u, T, cup);
    }
    // SS->node for d>0, node->TT for d<0
    // d[T] = 2*K  >0 : SS->T cap 2K
    // d[uni] = -2 : uni->TT cap 2
    long long needFlow=0;
    if(K>0){
        din.addEdge(SS, T, 2*K);
        needFlow += 2*K;
        for(int u=1;u<=K;u++){
            din.addEdge(N+u, TT, 2);
        }
    }
    // T->S inf
    din.addEdge(T,S,1000000);
    
    int f = din.maxflow(SS,TT);
    
    if(K>0 && f != needFlow){
        cout<<"NO\n";
        return 0;
    }
    
    // feasible
    cout<<"YES\n";
    // recover assignment: for each student edge, forward cap==0 means used
    vector<vector<int>> assign(K+1);
    for(auto &e : studEdges){
        int id=e[2];
        if(din.cap[id]==0){ // used
            assign[e[1]].push_back(e[0]);
        }
    }
    for(int u=1;u<=K;u++){
        cout<<assign[u].size();
        for(int x: assign[u]) cout<<" "<<x;
        cout<<"\n";
    }
    return 0;
}
