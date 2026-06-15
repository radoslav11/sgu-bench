#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int n;
vector<pair<int,ll>> adj[10001];
ll down1[10001], down2[10001];
int child1[10001];
ll up_dist[10001];
void dfs1(int u, int par) {
    down1[u] = down2[u] = 0; child1[u] = -1;
    for (auto& [v, w] : adj[u]) {
        if (v == par) continue;
        dfs1(v, u);
        ll cand = down1[v] + w;
        if (cand >= down1[u]) { down2[u]=down1[u]; down1[u]=cand; child1[u]=v; }
        else if (cand > down2[u]) down2[u]=cand;
    }
}
void dfs2(int u, int par) {
    for (auto& [v, w] : adj[u]) {
        if (v == par) continue;
        ll best = (child1[u]==v) ? down2[u] : down1[u];
        up_dist[v] = max(up_dist[u], best) + w;
        dfs2(v, u);
    }
}
int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    cin >> n;
    for (int i=2;i<=n;i++){ int p; ll w; cin>>p>>w; adj[i].push_back({p,w}); adj[p].push_back({i,w}); }
    up_dist[1]=0; dfs1(1,-1); dfs2(1,-1);
    for (int i=1;i<=n;i++) cout<<max(down1[i],up_dist[i])<<"\n";
}
