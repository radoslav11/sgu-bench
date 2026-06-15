#include <bits/stdc++.h>
using namespace std;
int n,a[500][500];
vector<int> adj[500];
int matchL[500],matchR[500],dist[500];
const int INF=1e9;
bool bfs(){queue<int>q;for(int u=0;u<n;u++){if(matchL[u]==-1){dist[u]=0;q.push(u);}else dist[u]=INF;}bool f=false;while(!q.empty()){int u=q.front();q.pop();for(int v:adj[u]){int w=matchR[v];if(w==-1)f=true;else if(dist[w]==INF){dist[w]=dist[u]+1;q.push(w);}}}return f;}
bool dfs(int u){for(int v:adj[u]){int w=matchR[v];if(w==-1||(dist[w]==dist[u]+1&&dfs(w))){matchL[u]=v;matchR[v]=u;return true;}}dist[u]=INF;return false;}
bool check(int T){for(int i=0;i<n;i++){adj[i].clear();for(int j=0;j<n;j++)if(a[i][j]<=T)adj[i].push_back(j);}fill(matchL,matchL+n,-1);fill(matchR,matchR+n,-1);int m=0;while(bfs())for(int u=0;u<n;u++)if(matchL[u]==-1&&dfs(u))m++;return m==n;}
int main(){ios::sync_with_stdio(false);cin.tie(0);cin>>n;vector<int>vals;for(int i=0;i<n;i++)for(int j=0;j<n;j++){cin>>a[i][j];vals.push_back(a[i][j]);}sort(vals.begin(),vals.end());vals.erase(unique(vals.begin(),vals.end()),vals.end());int lo=0,hi=(int)vals.size()-1,ans=hi;while(lo<=hi){int mid=(lo+hi)/2;if(check(vals[mid])){ans=mid;hi=mid-1;}else lo=mid+1;}check(vals[ans]);cout<<vals[ans]<<"\n";for(int i=0;i<n;i++)cout<<(i+1)<<" "<<(matchL[i]+1)<<"\n";}
