#include <bits/stdc++.h>
using namespace std;

int N, M, src, dst;
char initC[305];
int ri[305], tB[305], tP[305];

// color of junction i at time t: returns 0 for B, 1 for P
int colorAt(int i, long long t) {
    if (t < ri[i]) {
        return (initC[i] == 'B') ? 0 : 1;
    }
    long long rem = t - ri[i];
    int period = tB[i] + tP[i];
    int pos = rem % period;
    // after ri[i], color switches from initC[i]
    if (initC[i] == 'B') {
        // switches to P for tP[i], then B for tB[i], ...
        if (pos < tP[i]) return 1;
        else return 0;
    } else {
        // switches to B for tB[i], then P for tP[i], ...
        if (pos < tB[i]) return 0;
        else return 1;
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> src >> dst;
    cin >> N >> M;
    for (int i = 1; i <= N; i++) {
        cin >> initC[i] >> ri[i] >> tB[i] >> tP[i];
    }
    vector<vector<pair<int,int>>> adj(N+1);
    for (int k = 0; k < M; k++) {
        int u, v, l; cin >> u >> v >> l;
        adj[u].push_back({v, l});
        adj[v].push_back({u, l});
    }
    
    const long long INF = 1e18;
    vector<long long> dist(N+1, INF);
    vector<int> prev(N+1, -1);
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
    dist[src] = 0;
    pq.push({0, src});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        if (u == dst) break;
        for (auto [v, w] : adj[u]) {
            // find earliest t >= d where colorAt(u,t) == colorAt(v,t)
            long long found = -1;
            int periodU = tB[u] + tP[u], periodV = tB[v] + tP[v];
            long long limit = d + (long long)periodU * periodV + 500;
            for (long long t = d; t <= limit; t++) {
                if (colorAt(u, t) == colorAt(v, t)) { found = t; break; }
            }
            if (found == -1) continue;
            long long nd = found + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                prev[v] = u;
                pq.push({nd, v});
            }
        }
    }
    
    if (dist[dst] == INF) { cout << 0 << endl; return 0; }
    cout << dist[dst] << "\n";
    vector<int> path;
    for (int cur = dst; cur != -1; cur = prev[cur]) path.push_back(cur);
    reverse(path.begin(), path.end());
    for (int i = 0; i < (int)path.size(); i++) cout << path[i] << " \n"[i+1==(int)path.size()];
}
