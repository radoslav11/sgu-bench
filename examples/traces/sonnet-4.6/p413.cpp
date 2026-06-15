#include <bits/stdc++.h>
using namespace std;
int n,m,sz[105],color[105],country_cnt;
vector<int> tree[105];
void calc_size(int v,int par){sz[v]=1;for(int u:tree[v])if(u!=par){calc_size(u,v);sz[v]+=sz[u];}}
void assign_color(int v,int par,int col){color[v]=col;for(int u:tree[v])if(u!=par){if(sz[u]%2==0){country_cnt++;assign_color(u,v,country_cnt);}else assign_color(u,v,col);}}
int main(){int tst;cin>>tst;while(tst--){cin>>n>>m;for(int i=1;i<=n;i++)tree[i].clear();vector<vector<int>>adj(n+1);for(int i=0;i<m;i++){int a,b;cin>>a>>b;adj[a].push_back(b);adj[b].push_back(a);}vector<bool>vis(n+1,false);queue<int>q;q.push(1);vis[1]=true;while(!q.empty()){int v=q.front();q.pop();for(int u:adj[v])if(!vis[u]){vis[u]=true;tree[v].push_back(u);tree[u].push_back(v);q.push(u);}}calc_size(1,0);country_cnt=1;assign_color(1,0,1);for(int i=1;i<=n;i++)cout<<color[i]<<" \n"[i==n];}}</p>
