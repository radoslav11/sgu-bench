#include<bits/stdc++.h>
using namespace std;
struct Edge{int to,rev,cap,eid;};
vector<Edge>g[55];
int lv[55],it[55];
void ae(int u,int v,int c,int id){g[u].push_back({v,(int)g[v].size(),c,id});g[v].push_back({u,(int)g[u].size()-1,c,id});}
bool bfs(int s,int t){fill(lv,lv+55,-1);queue<int>q;lv[s]=0;q.push(s);while(!q.empty()){int v=q.front();q.pop();for(auto&e:g[v])if(e.cap>0&&lv[e.to]<0){lv[e.to]=lv[v]+1;q.push(e.to);}}return lv[t]>=0;}
int dfs(int v,int t,int f){if(v==t)return f;for(int&i=it[v];i<(int)g[v].size();i++){auto&e=g[v][i];if(e.cap>0&&lv[v]<lv[e.to]){int d=dfs(e.to,t,min(f,e.cap));if(d>0){e.cap-=d;g[e.to][e.rev].cap+=d;return d;}}}return 0;}
int mf(int s,int t){int r=0;while(bfs(s,t)){fill(it,it+55,0);int d;while((d=dfs(s,t,1<<29))>0)r+=d;}return r;}
int main(){
    int N,M;scanf("%d%d",&N,&M);
    vector<int>A(M),B(M),W(M);
    for(int i=0;i<M;i++)scanf("%d%d%d",&A[i],&B[i],&W[i]);
    int best=INT_MAX,bv=-1,bs=-1,bt=-1;
    for(int v=1;v<=N;v++){
        vector<int>nd;for(int i=1;i<=N;i++)if(i!=v)nd.push_back(i);
        if((int)nd.size()<2)continue;
        int s=nd[0];
        for(int ti=1;ti<(int)nd.size();ti++){
            int t=nd[ti];
            for(int i=0;i<=N;i++)g[i].clear();
            for(int i=0;i<M;i++){if(A[i]==v||B[i]==v)continue;ae(A[i],B[i],W[i],i);}
            int f=mf(s,t);
            if(f<best){best=f;bv=v;bs=s;bt=t;}
        }
    }
    // rerun best
    for(int i=0;i<=N;i++)g[i].clear();
    for(int i=0;i<M;i++){if(A[i]==bv||B[i]==bv)continue;ae(A[i],B[i],W[i],i);}
    mf(bs,bt);
    // find reachable from bs in residual
    vector<bool>vis(N+1,false);
    queue<int>q;q.push(bs);vis[bs]=true;
    while(!q.empty()){int u=q.front();q.pop();for(auto&e:g[u])if(e.cap>0&&!vis[e.to]){vis[e.to]=true;q.push(e.to);}}
    vector<int>cut;
    set<int>seen;
    for(int u=1;u<=N;u++)if(u!=bv&&vis[u])for(auto&e:g[u])if(e.eid>=0&&!vis[e.to]&&e.to!=bv)if(!seen.count(e.eid)){seen.insert(e.eid);cut.push_back(e.eid+1);}
    printf("%d\n%d\n",best,(int)cut.size());
    for(int i=0;i<(int)cut.size();i++)printf("%d%c",cut[i],i+1==(int)cut.size()?'\n':' ');
    if(cut.empty())printf("\n");
}
