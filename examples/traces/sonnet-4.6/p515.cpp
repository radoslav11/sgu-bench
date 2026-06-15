#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll,int> pli;
const ll INF=1e18;
int n,m;
vector<pair<int,int>> g[100005];
int ea[100005],eb[100005],et[100005];
vector<ll> dijkstra(int src,vector<int>&par,vector<int>&parEdge){par.assign(n+1,-1);parEdge.assign(n+1,-1);vector<ll>d(n+1,INF);priority_queue<pli,vector<pli>,greater<pli>>pq;d[src]=0;pq.push({0,src});while(!pq.empty()){auto[dd,u]=pq.top();pq.pop();if(dd>d[u])continue;for(auto[v,ei]:g[u]){ll nd=dd+et[ei];if(nd<d[v]){d[v]=nd;par[v]=u;parEdge[v]=ei;pq.push({nd,v});}}}return d;}
int main(){
scanf("%d%d",&n,&m);
for(int i=1;i<=m;i++){scanf("%d%d%d",&ea[i],&eb[i],&et[i]);g[ea[i]].push_back({eb[i],i});g[eb[i]].push_back({ea[i],i});}
int k;scanf("%d",&k);
vector<int>cities(k);for(int i=0;i<k;i++)scanf("%d",&cities[i]);
vector<vector<ll>>dist(k);
vector<vector<int>>par(k),parEdge(k);
for(int i=0;i<k;i++)dist[i]=dijkstra(cities[i],par[i],parEdge[i]);
// find S,T
int si=0,ti=1;ll best=0;
for(int i=0;i<k;i++)for(int j=i+1;j<k;j++){ll d=dist[i][cities[j]];if(d>best){best=d;si=i;ti=j;}}
// order cities by dist[si][city]
vector<int>order(k);iota(order.begin(),order.end(),0);
sort(order.begin(),order.end(),[&](int a,int b){return dist[si][cities[a]]<dist[si][cities[b]];});
// reconstruct path
vector<int>ans;
for(int i=0;i+1<k;i++){
int u=cities[order[i]],v=cities[order[i+1]];
// trace from v back to u using par[order[i]]
vector<int>seg;int cur=v;
while(cur!=u){seg.push_back(parEdge[order[i]][cur]);cur=par[order[i]][cur];}
reverse(seg.begin(),seg.end());
for(int e:seg)ans.push_back(e);
}
printf("%d\n",(int)ans.size());
for(int i=0;i<(int)ans.size();i++)printf("%d%c",ans[i]," \n"[i+1==(int)ans.size()]);
if(ans.empty())printf("\n");
}
