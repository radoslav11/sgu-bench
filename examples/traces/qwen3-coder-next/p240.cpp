#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Edge {
    int v, T, R, P;
    Edge(int v, int T, int R, int P) : v(v), T(T), R(R), P(P) {}
};

vector<vector<Edge>> adj;
vector<bool> is_exit;

bool check(ll X, int N, int S, vector<int>& parent, int& found_exit) {
    vector<ll> dist(N+1, LLONG_MAX);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    parent.assign(N+1, -1);

    dist[S] = 0;
    pq.push({0, S});

    while (!pq.empty()) {
        ll t = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (t != dist[u]) continue;

        if (is_exit[u]) {
            found_exit = u;
            return true;
        }

        for (const Edge& e : adj[u]) {
            ll arrival = t + e.T;
            ll temp_at_arrival = e.R + (ll)e.P * arrival;
            if (temp_at_arrival <= X) {
                if (arrival < dist[e.v]) {
                    dist[e.v] = arrival;
                    parent[e.v] = u;
                    pq.push({arrival, e.v});
                }
            }
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N, M, H, S, E_count;
    cin >> N >> M >> H >> S >> E_count;

    adj.resize(N+1);
    for (int i = 0; i < M; i++) {
        int a, b, t, r, p;
        cin >> a >> b >> t >> r >> p;
        adj[a].push_back(Edge(b, t, r, p));
        adj[b].push_back(Edge(a, t, r, p));
    }

    is_exit.assign(N+1, false);
    for (int i = 0; i < E_count; i++) {
        int exit_node;
        cin >> exit_node;
        is_exit[exit_node] = true;
    }

    vector<int> parent;
    int found_exit_node = -1;
    ll best_X = H + 1;

    if (is_exit[S]) {
        best_X = 0;
        parent.assign(N+1, -1);
        parent[S] = -1;
        found_exit_node = S;
    } else {
        ll low = 0, high = 20000000000LL;
        while (low < high) {
            ll mid = (low + high) / 2;
            if (check(mid, N, S, parent, found_exit_node)) {
                high = mid;
            } else {
                low = mid + 1;
            }
        }
        best_X = low;
        if (!check(best_X, N, S, parent, found_exit_node)) {
            cout << "NO\n";
            return 0;
        }
    }

    if (best_X > H) {
        cout << "NO\n";
        return 0;
    }

    vector<int> path;
    int cur = found_exit_node;
    while (cur != -1) {
        path.push_back(cur);
        cur = parent[cur];
    }
    reverse(path.begin(), path.end());

    cout << "YES\n";
    cout << best_X << "\n";
    cout << path.size();
    for (int node : path) {
        cout << " " << node;
    }
    cout << "\n";

    return 0;
}
