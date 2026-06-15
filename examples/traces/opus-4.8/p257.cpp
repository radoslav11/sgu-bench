#include<bits/stdc++.h>
using namespace std;
struct Dinic{
 struct E{int to,cap,rev;};
 vector<vector<E>> g;vector<int> lvl,it;int n;
 Dinic(int n):g(n),lvl(n),it(n),n(n){}
 void add(int a,int b,int c){g[a].push_back({b,c,(int)g[b].size()});g[b].push_back({a,0,(int)g[a].size()-1});}
 bool bfs(int s,int t){fill(lvl.begin(),lvl.end(),-1);queue<int>q;lvl[s]=0;q.push(s);while(!q.empty()){int v=q.front();q.pop();for(auto&e:g[v])if(e.cap>0&&lvl[e.to]<0){lvl[e.to]=lvl[v]+1;q.push(e.to);}}return lvl[t]>=0;}
 int dfs(int v,int t,int f){if(v==t)return f;for(int&i=it[v];i<(int)g[v].size();i++){E&e=g[v][i];if(e.cap>0&&lvl[v]<lvl[e.to]){int d=dfs(e.to,t,min(f,e.cap));if(d>0){e.cap-=d;g[e.to][e.rev].cap+=d;return d;}}}return 0;}
 int maxflow(int s,int t){int fl=0;while(bfs(s,t)){fill(it.begin(),it.end(),0);int f;while((f=dfs(s,t,INT_MAX))>0)fl+=f;}return fl;}
};
int main(){
 int P,O,Sd;scanf("%d %d %d",&P,&O,&Sd);
 int N;scanf("%d",&N);
 vector<string> c(N);
 for(int i=0;i<N;i++){char buf[10];scanf("%s",buf);c[i]=buf;}
 int S=0,sink=4+N;
 Dinic d(4+N+1);
 long long dem[3]={(long long)P,(long long)O,(long long)Sd};
 for(int p=0;p<3;p++){d.add(1+p,sink,(int)dem[p]);}
 for(int i=0;i<N;i++){
  d.add(S,4+i,2);
  for(int p=0;p<3;p++){int val=c[i][p]=='B'?2:1;d.add(4+i,1+p,val);}
 }
 int need=P+O+Sd;
 int f=d.maxflow(S,sink);
 if(f!=need){printf("no solution\n");return 0;}
 string res(N,'?');
 char nm[3]={'P','O','S'};
 for(int i=0;i<N;i++){
  int node=4+i;int best=-1,bestf=0;
  for(auto&e:d.g[node]){
   if(e.to>=1&&e.to<=3){
    int orig=c[i][e.to-1]=='B'?2:1;int fl=orig-e.cap;if(fl>bestf){bestf=fl;best=e.to-1;}
   }
  }
  if(best<0)best=0;
  res[i]=nm[best];
 }
 printf("%s\n",res.c_str());
 return 0;
}
