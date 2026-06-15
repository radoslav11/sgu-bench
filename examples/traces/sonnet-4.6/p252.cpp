#include <bits/stdc++.h>
using namespace std;
const int INF=1e9;
struct Edge{int to,cap,cost,flow;};
vector<Edge> edges;
vector<vector<int>> g;
int nn;
void addEdge(int u,int v,int cap,int cost){
    g[u].push_back(edges.size());
    edges.push_back({v,cap,cost,0});
    g[v].push_back(edges.size());
    edges.push_back({u,0,-cost,0});
}
pair<int,int> mcmf(int s,int t){
    int fl=0,co=0;
    while(true){
        vector<int>d(nn,INF),inq(nn,0),pv(nn,-1),pe(nn,-1);
        d[s]=0;
        deque<int>q;
        q.push_back(s);inq[s]=1;
        while(!q.empty()){
            int v=q.front();q.pop_front();inq[v]=0;
            for(int id:g[v]){
                auto&e=edges[id];
                if(e.cap>e.flow&&d[v]+e.cost<d[e.to]){
                    d[e.to]=d[v]+e.cost;
                    pv[e.to]=v;pe[e.to]=id;
                    if(!inq[e.to]){inq[e.to]=1;q.push_back(e.to);}
                }
            }
        }
        if(d[t]==INF)break;
        int v=t;
        while(v!=s){edges[pe[v]].flow++;edges[pe[v]^1].flow--;v=pv[v];}
        fl++;co+=d[t];
    }
    return{fl,co};
}
int main(){
    int N,M;
    scanf("%d%d",&N,&M);
    nn=2*N+2;
    int S=0,T=2*N+1;
    g.resize(nn);
    for(int i=1;i<=N;i++){
        addEdge(S,i,1,0);
        addEdge(N+i,T,1,0);
    }
    for(int i=0;i<M;i++){
        int a,b,c;
        scanf("%d%d%d",&a,&b,&c);
        addEdge(a,N+b,1,c);
    }
    auto [fl,co]=mcmf(S,T);
    printf("%d %d\n",N-fl,co);
    vector<int>nxt(N+1,-1);
    for(int u=1;u<=N;u++){
        for(int id:g[u]){
            auto&e=edges[id];
            int v=e.to-N;
            if(v>=1&&v<=N&&e.flow==1){
                nxt[u]=v;
            }
        }
    }
    vector<bool>has_prev(N+1,false);
    for(int u=1;u<=N;u++)if(nxt[u]!=-1)has_prev[nxt[u]]=true;
    for(int u=1;u<=N;u++){
        if(!has_prev[u]){
            vector<int>path;
            int cur=u;
            while(cur!=-1){path.push_back(cur);cur=nxt[cur];}
            printf("%d",(int)path.size());
            for(int x:path)printf(" %d",x);
            printf("\n");
        }
    }
}
