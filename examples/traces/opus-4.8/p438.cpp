#include <bits/stdc++.h>
using namespace std;
struct Dinic{
  struct E{int to,cap,rev;};
  vector<vector<E>> g; vector<int> level,iter;
  Dinic(int n=0){g.resize(n);}
  void add(int a,int b,int c){g[a].push_back({b,c,(int)g[b].size()});g[b].push_back({a,0,(int)g[a].size()-1});}
  bool bfs(int s,int t){level.assign(g.size(),-1);queue<int>q;level[s]=0;q.push(s);while(!q.empty()){int v=q.front();q.pop();for(auto&e:g[v])if(e.cap>0&&level[e.to]<0){level[e.to]=level[v]+1;q.push(e.to);}}return level[t]>=0;}
  int dfs(int v,int t,int f){if(v==t)return f;for(int&i=iter[v];i<(int)g[v].size();i++){E&e=g[v][i];if(e.cap>0&&level[v]<level[e.to]){int d=dfs(e.to,t,min(f,e.cap));if(d>0){e.cap-=d;g[e.to][e.rev].cap+=d;return d;}}}return 0;}
  int maxflow(int s,int t){int fl=0;while(bfs(s,t)){iter.assign(g.size(),0);int f;while((f=dfs(s,t,INT_MAX))>0)fl+=f;}return fl;}
};
int main(){
  int N,M,D,W; cin>>N>>M>>D>>W;
  vector<int> X(N),Y(N),C(N);
  for(int i=0;i<N;i++)cin>>X[i]>>Y[i]>>C[i];
  if(W<=D){cout<<1<<endl;return 0;}
  int MAXT=300;
  int total=2+MAXT*N*2;
  Dinic d(total);
  int S=0,T=1;
  auto in=[&](int t,int p){return 2+(t*N+p)*2;};
  auto out=[&](int t,int p){return 2+(t*N+p)*2+1;};
  auto sq=[](long long x){return x*x;};
  int flow=0;
  for(int t=1;t<MAXT;t++){
    for(int p=0;p<N;p++){
      d.add(in(t,p),out(t,p),C[p]);
      if(t==1){ if((long long)Y[p]<=D) d.add(S,in(t,p),INT_MAX); }
      if(t>1){ for(int q=0;q<N;q++){ if(sq(X[p]-X[q])+sq(Y[p]-Y[q])<=(long long)D*D) d.add(out(t-1,q),in(t,p),INT_MAX);} }
      if((long long)(W-Y[p])<=D) d.add(out(t,p),T,INT_MAX);
    }
    flow+=d.maxflow(S,T);
    if(flow>=M){cout<<t<<endl;return 0;}
  }
  cout<<"IMPOSSIBLE"<<endl;
}
