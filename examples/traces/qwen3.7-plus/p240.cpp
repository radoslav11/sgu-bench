#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Edge {
    int to;
    long long T, R, P;
};

int N, M;
long long H;
int S, E_count;
vector<vector<Edge>> adj;
vector<bool> is_exit;

bool check(long long X, vector<int>& best_path) {
    vector<long long> dist(N + 1, -1);
    vector<int> parent(N + 1, -1);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    
    dist[S] = 0;
    pq.push({0, S});
    
    while (!pq.empty()) {
        auto [t, u] = pq.top();
        pq.pop();
        
        if (t > dist[u]) continue;
        
        if (is_exit[u]) {
            vector<int> path;
            int curr = u;
            while (curr != -1) {
                path.push_back(curr);
                curr = parent[curr];
            }
            reverse(path.begin(), path.end());
            best_path = path;
            return true;
        }
        
        for (const auto& edge : adj[u]) {
            int v = edge.to;
            long long t_next = t + edge.T;
            long long heat = edge.R + edge.P * t_next;
            if (heat <= X && (dist[v] == -1 || t_next < dist[v])) {
                dist[v] = t_next;
                parent[v] = u;
                pq.push({t_next, v});
            }
        }
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    if (!(cin >> N >> M >> H >> S >> E_count)) return 0;
    
    adj.resize(N + 1);
    is_exit.assign(N + 1, false);
    
    for (int i = 0; i < M; ++i) {
        int u, v;
        long long t, r, p;
        cin >> u >> v >> t >> r >> p;
        adj[u].push_back({v, t, r, p});
        adj[v].push_back({u, t, r, p});
    }
    
    for (int i = 0; i < E_count; ++i) {
        int f;
        cin >> f;
        is_exit[f] = true;
    }
    
    long long low = 0, high = 1000000000000000LL; // 10^15 is safely above max possible heat (~10^10)
    long long ans = -1;
    vector<int> final_path;
    
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        vector<int> current_path;
        if (check(mid, current_path)) {
            ans = mid;
            final_path = current_path;
            high = mid - 1; // Try to find a smaller maximum heat
        } else {
            low = mid + 1;
        }
    }
    
    if (ans == -1 || ans > H) {
        cout << "NO\n";
    } else {
        cout << "YES\n";
        cout << ans << "\n";
        cout << final_path.size();
        for (int node : final_path) {
            cout << " " << node;
        }
        cout << "\n";
    }
    
    return 0;
}
