#include <bits/stdc++.h>
using namespace std;
int par[3005][13];
double maxEdge[3005][13];
int depth[3005];
vector<pair<int,double>> adj[3005];
void dfs(int u,int p,int d){depth[u]=d;for(auto[v,w]:adj[u]){if(v==p)continue;par[v][0]=u;maxEdge[v][0]=w;for(int k=1;k<13;k++){par[v][k]=par[par[v][k-1]][k-1];maxEdge[v][k]=max(maxEdge[v][k-1],maxEdge[par[v][k-1]][k-1]);}dfs(v,u,d+1);}}
double query(int u,int v){double res=0;if(depth[u]<depth[v])swap(u,v);int diff=depth[u]-depth[v];for(int k=0;k<13;k++)if((diff>>k)&1){res=max(res,maxEdge[u][k]);u=par[u][k];}if(u==v)return res;for(int k=12;k>=0;k--)if(par[u][k]!=par[v][k]){res=max(res,max(maxEdge[u][k],maxEdge[v][k]));u=par[u][k];v=par[v][k];}res=max(res,max(maxEdge[u][0],maxEdge[v][0]));return res;}
int main(){ios::sync_with_stdio(false);cin.tie(0);int n;cin>>n;vector<double>x(n+1),y(n+1);for(int i=1;i<=n;i++)cin>>x[i]>>y[i];vector<double>key(n+1,1e18);vector<int>mstPar(n+1,-1);vector<bool>inMST(n+1,false);key[1]=0;for(int iter=0;iter<n;iter++){int u=-1;for(int i=1;i<=n;i++)if(!inMST[i]&&(u==-1||key[i]<key[u]))u=i;inMST[u]=true;if(mstPar[u]!=-1){double w=key[u];adj[u].push_back({mstPar[u],w});adj[mstPar[u]].push_back({u,w});}for(int v=1;v<=n;v++)if(!inMST[v]){double d=hypot(x[u]-x[v],y[u]-y[v]);if(d<key[v]){key[v]=d;mstPar[v]=u;}}}par[1][0]=1;maxEdge[1][0]=0;for(int k=1;k<13;k++){par[1][k]=1;maxEdge[1][k]=0;}dfs(1,-1,0);int q;cin>>q;cout<<fixed<<setprecision(10);while(q--){int s,t;cin>>s>>t;cout<<query(s,t)<<"\n";}}</pre>
