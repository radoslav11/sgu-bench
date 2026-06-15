#include <bits/stdc++.h>
using namespace std;

struct Dinic {
    struct E { int to, cap, rev; };
    vector<vector<E>> g;
    vector<int> level, iter;
    int n;
    Dinic(int n): n(n), g(n), level(n), iter(n) {}
    void add(int u,int v,int c){
        g[u].push_back({v,c,(int)g[v].size()});
        g[v].push_back({u,c,(int)g[u].size()-1}); // undirected
    }
    bool bfs(int s,int t){
        fill(level.begin(),level.end(),-1);
        queue<int>q; level[s]=0; q.push(s);
        while(!q.empty()){int u=q.front();q.pop();
            for(auto&e:g[u]) if(e.cap>0&&level[e.to]<0){level[e.to]=level[u]+1;q.push(e.to);}}
        return level[t]>=0;
    }
    int dfs(int u,int t,int f){
        if(u==t)return f;
        for(int&i=iter[u];i<(int)g[u].size();i++){
            E&e=g[u][i];
            if(e.cap>0&&level[u]<level[e.to]){
                int d=dfs(e.to,t,min(f,e.cap));
                if(d>0){e.cap-=d;g[e.to][e.rev].cap+=d;return d;}
            }
        }
        return 0;
    }
    int maxflow(int s,int t){
        int flow=0;
        while(bfs(s,t)){
            fill(iter.begin(),iter.end(),0);
            int f;
            while((f=dfs(s,t,INT_MAX))>0)flow+=f;
        }
        return flow;
    }
};

int main(){
    int N,M; scanf("%d %d",&N,&M);
    vector<int> A(M),B(M),W(M);
    for(int i=0;i<M;i++) scanf("%d %d %d",&A[i],&B[i],&W[i]);
    
    int bestCost=INT_MAX;
    vector<int> bestEdges;
    
    for(int v=1; v<=N; v++){
        int s0=-1;
        for(int i=1;i<=N;i++) if(i!=v){s0=i;break;}
        for(int t=1;t<=N;t++){
            if(t==v||t==s0) continue;
            Dinic d(N+1);
            for(int i=0;i<M;i++){
                if(A[i]!=v && B[i]!=v) d.add(A[i],B[i],W[i]);
            }
            int f=d.maxflow(s0,t);
            if(f<bestCost){
                vector<int> reach(N+1,0);
                queue<int>q; reach[s0]=1; q.push(s0);
                while(!q.empty()){int u=q.front();q.pop();
                    for(auto&e:d.g[u]) if(e.cap>0&&!reach[e.to]){reach[e.to]=1;q.push(e.to);}}
                vector<int> edges;
                for(int i=0;i<M;i++){
                    if(A[i]!=v && B[i]!=v && reach[A[i]]!=reach[B[i]]) edges.push_back(i+1);
                }
                bestCost=f; bestEdges=edges;
            }
        }
    }
    
    printf("%d\n",bestCost);
    printf("%d\n",(int)bestEdges.size());
    for(int i=0;i<(int)bestEdges.size();i++) printf("%d%c",bestEdges[i]," \n"[i+1==(int)bestEdges.size()]);
    if(bestEdges.empty()) printf("\n");
    return 0;
}
