#include <bits/stdc++.h>
using namespace std;
const int INF=1e9;
struct E{int to,cap,flow;};
vector<E> ed; vector<int> g[205]; int level[205],it[205],n_;
void ae(int u,int v,int c){g[u].push_back(ed.size());ed.push_back({v,c,0});g[v].push_back(ed.size());ed.push_back({u,0,0});}
bool bfs(int s,int t){for(int i=0;i<n_;i++)level[i]=-1;queue<int>q;q.push(s);level[s]=0;while(!q.empty()){int u=q.front();q.pop();for(int id:g[u]){if(level[ed[id].to]<0&&ed[id].cap>ed[id].flow){level[ed[id].to]=level[u]+1;q.push(ed[id].to);}}}return level[t]>=0;}
int dfs(int u,int t,int f){if(u==t)return f;for(int&i=it[u];i<(int)g[u].size();i++){int id=g[u][i];E&e=ed[id];if(level[e.to]==level[u]+1&&e.cap>e.flow){int d=dfs(e.to,t,min(f,e.cap-e.flow));if(d>0){e.flow+=d;ed[id^1].flow-=d;return d;}}}return 0;}
int maxflow(int s,int t){int fl=0;while(bfs(s,t)){for(int i=0;i<n_;i++)it[i]=0;int f;while((f=dfs(s,t,INF))>0)fl+=f;}return fl;}
int main(){int N,M;scanf("%d %d",&N,&M);n_=N+2;int SS=N,TT=N+1;
vector<int> low(M),eid(M);long long need=0;
for(int i=0;i<M;i++){int u,v,z,c;scanf("%d %d %d %d",&u,&v,&z,&c);u--;v--;int lo=c?z:0;low[i]=lo;eid[i]=ed.size();ae(u,v,z-lo);if(lo){ae(SS,v,lo);ae(u,TT,lo);need+=lo;}}
int retEdge=ed.size();ae(N-1,0,INF);
maxflow(SS,TT);
long long got=0;for(int id:g[SS])got+=ed[id].flow;
if(got!=need){printf("Impossible\n");return 0;}
int curRet=ed[retEdge].flow;ed[retEdge].cap=0;ed[retEdge].flow=0;ed[retEdge^1].flow=0;
int back=maxflow(N-1,0);
int speed=curRet-back;
printf("%d\n",speed);
for(int i=0;i<M;i++){int f=low[i]+ed[eid[i]].flow;printf("%d%c",f,i+1<M?' ':'\n');}
if(M==0)printf("\n");
return 0;}
