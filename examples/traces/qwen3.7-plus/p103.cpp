#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;

const int INF = 1e9;

int N, M;
int source, dest;
char init_color[305];
int rem_time[305];
int tB[305], tP[305];

struct Edge {
    int to;
    int len;
};
vector<vector<Edge>> adj;

char get_color(int u, int t) {
    if (t < rem_time[u]) {
        return init_color[u];
    }
    int time_prime = t - rem_time[u];
    int cycle = tB[u] + tP[u];
    int rem = time_prime % cycle;
    char next_c = (init_color[u] == 'B') ? 'P' : 'B';
    int next_t = (init_color[u] == 'B') ? tP[u] : tB[u];
    if (rem < next_t) {
        return next_c;
    } else {
        return init_color[u];
    }
}

int time_to_next_change(int u, int t) {
    if (t < rem_time[u]) {
        return rem_time[u] - t;
    }
    int time_prime = t - rem_time[u];
    int cycle = tB[u] + tP[u];
    int rem = time_prime % cycle;
    int next_t = (init_color[u] == 'B') ? tP[u] : tB[u];
    if (rem < next_t) {
        return next_t - rem;
    } else {
        int current_phase_len = (init_color[u] == 'B') ? tB[u] : tP[u];
        return current_phase_len - (rem - next_t);
    }
}

int next_matching_time(int u, int v, int t) {
    for (int step = 0; step < 10000; ++step) {
        char cu = get_color(u, t);
        char cv = get_color(v, t);
        if (cu == cv) return t;
        
        int rem_u = time_to_next_change(u, t);
        int rem_v = time_to_next_change(v, t);
        
        int jump = min(rem_u, rem_v);
        if (jump <= 0) jump = 1; // Safety fallback
        
        t += jump;
    }
    return -1;
}

void solve() {
    if (!(cin >> source >> dest)) return;
    cin >> N >> M;
    for (int i = 1; i <= N; ++i) {
        cin >> init_color[i] >> rem_time[i] >> tB[i] >> tP[i];
    }
    adj.resize(N + 1);
    for (int i = 0; i < M; ++i) {
        int u, v, l;
        cin >> u >> v >> l;
        adj[u].push_back({v, l});
        adj[v].push_back({u, l});
    }

    vector<int> dist(N + 1, INF);
    vector<int> parent(N + 1, -1);
    
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    dist[source] = 0;
    pq.push({0, source});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (d > dist[u]) continue;
        if (u == dest) break;
        
        for (const auto& edge : adj[u]) {
            int v = edge.to;
            int len = edge.len;
            
            int nxt = next_matching_time(u, v, d);
            if (nxt != -1) {
                int new_dist = nxt + len;
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                    parent[v] = u;
                    pq.push({new_dist, v});
                }
            }
        }
    }
    
    if (dist[dest] == INF) {
        cout << 0 << "\n";
    } else {
        cout << dist[dest] << "\n";
        vector<int> path;
        int curr = dest;
        while (curr != -1) {
            path.push_back(curr);
            curr = parent[curr];
        }
        reverse(path.begin(), path.end());
        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i] << (i + 1 == path.size() ? "" : " ");
        }
        cout << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
