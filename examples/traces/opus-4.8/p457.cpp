#include <bits/stdc++.h>
using namespace std;

struct Dinic{
    struct E{int to,cap;};
    vector<E> es;
    vector<vector<int>> g;
    vector<int> level, iter;
    int n;
    Dinic(int n):n(n),g(n),level(n),iter(n){}
    int add(int u,int v,int c){
        int id=es.size();
        es.push_back({v,c}); g[u].push_back(id);
        es.push_back({u,0}); g[v].push_back(id+1);
        return id;
    }
    bool bfs(int s,int t){
        fill(level.begin(),level.end(),-1);
        queue<int>q; level[s]=0; q.push(s);
        while(!q.empty()){int u=q.front();q.pop();
            for(int id:g[u]){auto&e=es[id]; if(e.cap>0&&level[e.to]<0){level[e.to]=level[u]+1;q.push(e.to);}}}
        return level[t]>=0;
    }
    int dfs(int u,int t,int f){
        if(u==t)return f;
        for(int&i=iter[u];i<(int)g[u].size();i++){
            int id=g[u][i]; auto&e=es[id];
            if(e.cap>0&&level[e.to]==level[u]+1){
                int d=dfs(e.to,t,min(f,e.cap));
                if(d>0){es[id].cap-=d; es[id^1].cap+=d; return d;}
            }
        }
        return 0;
    }
    int maxflow(int s,int t){
        int flow=0;
        while(bfs(s,t)){
            fill(iter.begin(),iter.end(),0);
            int f;
            while((f=dfs(s,t,INT_MAX))>0) flow+=f;
        }
        return flow;
    }
};

int main(){
    int n,m,A,B;
    scanf("%d %d %d %d",&n,&m,&A,&B);
    vector<int> ru(m),rv(m),rw(m),rt(m);
    for(int i=0;i<m;i++) scanf("%d %d %d %d",&ru[i],&rv[i],&rw[i],&rt[i]);
    
    const int INF=1000000000;
    int SS=0, TT=n+1; // nodes 1..n
    Dinic din(n+2);
    vector<int> edgeId(m), low(m), cap0(m);
    vector<long long> excess(n+2,0);
    for(int i=0;i<m;i++){
        int l = (rt[i]==1)? rw[i] : 0;
        int c = rw[i];
        cap0[i]=c-l; low[i]=l;
        edgeId[i]=din.add(ru[i],rv[i],c-l);
        excess[rv[i]]+=l; excess[ru[i]]-=l;
    }
    int ba_id = din.add(B,A,INF);
    long long need=0;
    for(int x=1;x<=n;x++){
        if(excess[x]>0){ din.add(SS,x,(int)excess[x]); need+=excess[x]; }
        else if(excess[x]<0){ din.add(x,TT,(int)(-excess[x])); }
    }
    
    int f = din.maxflow(SS,TT);
    if(f!=need){
        // infeasible
        printf("0\n");
        return 0;
    }
    // flow on B->A edge
    int f0 = INF - din.es[ba_id].cap;
    // remove B->A edge
    din.es[ba_id].cap=0;
    din.es[ba_id^1].cap=0;
    int f1 = din.maxflow(A,B);
    int p = f0 + f1;
    
    if(p<=0){
        printf("0\n");
        return 0;
    }
    
    // recover flows on roads
    vector<int> flow(m);
    for(int i=0;i<m;i++){
        flow[i] = low[i] + (cap0[i] - din.es[edgeId[i]].cap);
    }
    
    // build multigraph for Euler
    vector<vector<pair<int,int>>> adj(n+1); // (to, isVirtual)
    for(int i=0;i<m;i++){
        for(int k=0;k<flow[i];k++) adj[ru[i]].push_back({rv[i],0});
    }
    for(int k=0;k<p;k++) adj[B].push_back({A,1});
    
    vector<int> it(n+1,0);
    vector<int> stV, stVirt;
    stV.push_back(A); stVirt.push_back(-1);
    vector<int> resV, resVirt;
    while(!stV.empty()){
        int u=stV.back();
        if(it[u]<(int)adj[u].size()){
            auto pr=adj[u][it[u]++];
            stV.push_back(pr.first); stVirt.push_back(pr.second);
        } else {
            resV.push_back(u); resVirt.push_back(stVirt.back());
            stV.pop_back(); stVirt.pop_back();
        }
    }
    reverse(resV.begin(),resV.end());
    reverse(resVirt.begin(),resVirt.end());
    
    vector<vector<int>> routes;
    vector<int> cur;
    if(!resV.empty()) cur.push_back(resV[0]);
    for(int i=1;i<(int)resV.size();i++){
        if(resVirt[i]==1){
            routes.push_back(cur);
            cur.clear();
            cur.push_back(resV[i]);
        } else {
            cur.push_back(resV[i]);
        }
    }
    
    // output
    printf("%d\n",(int)routes.size());
    // use buffer
    string out;
    out.reserve(1<<20);
    char buf[16];
    for(auto&r:routes){
        for(size_t j=0;j<r.size();j++){
            int len=sprintf(buf,"%d",r[j]);
            out.append(buf,len);
            out.push_back(' ');
        }
        out.push_back('\n');
    }
    fputs(out.c_str(),stdout);
    return 0;
}
