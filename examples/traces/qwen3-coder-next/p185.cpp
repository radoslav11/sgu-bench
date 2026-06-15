#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to, capacity;
    long long cost;
    int rev;
};

void dijkstra(int start, vector<long long>& dist, vector<vector<pair<int, int>>>& graph) {
    int n = graph.size() - 1;
    dist.assign(n + 1, LLONG_MAX);
    dist[start] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    pq.push({0, start});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue;
        for (auto [v, w] : graph[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<vector<pair<int, int>>> orig(N + 1);
    vector<tuple<int, int, int>> edges;
    for (int i = 0; i < M; i++) {
        int x, y, l;
        cin >> x >> y >> l;
        orig[x].push_back({y, l});
        orig[y].push_back({x, l});
        edges.push_back(make_tuple(x, y, l));
    }

    vector<long long> dist1, distN;
    dijkstra(1, dist1, orig);
    dijkstra(N, distN, orig);

    if (dist1[N] == LLONG_MAX) {
        cout << "No solution" << endl;
        return 0;
    }
    long long D = dist1[N];

    vector<vector<Edge>> g(N + 1);

    auto add_edge = [&](int u, int v, int cap, long long cost) {
        g[u].push_back({v, cap, cost, (int)g[v].size()});
        g[v].push_back({u, 0, -cost, (int)g[u].size() - 1});
    };

    for (auto [x, y, l] : edges) {
        if (dist1[x] != LLONG_MAX && distN[y] != LLONG_MAX && dist1[x] + l + distN[y] == D) {
            add_edge(x, y, 1, l);
        }
        if (dist1[y] != LLONG_MAX && distN[x] != LLONG_MAX && dist1[y] + l + distN[x] == D) {
            add_edge(y, x, 1, l);
        }
    }

    vector<vector<int>> paths;
    vector<long long> potential(N + 1, 0);
    vector<int> prev_node(N + 1), prev_edge(N + 1);
    vector<long long> dist(N + 1);

    auto dijkstra_flow = [&]() -> bool {
        fill(dist.begin(), dist.end(), LLONG_MAX);
        dist[1] = 0;
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
        pq.push({0, 1});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d != dist[u]) continue;
            for (int i = 0; i < (int)g[u].size(); i++) {
                auto &e = g[u][i];
                if (e.capacity <= 0) continue;
                long long nd = d + e.cost + potential[u] - potential[e.to];
                if (nd < dist[e.to]) {
                    dist[e.to] = nd;
                    prev_node[e.to] = u;
                    prev_edge[e.to] = i;
                    pq.push({nd, e.to});
                }
            }
        }
        return dist[N] != LLONG_MAX;
    };

    bool found = true;
    for (int iter = 0; iter < 2; iter++) {
        if (!dijkstra_flow()) {
            found = false;
            break;
        }

        for (int i = 1; i <= N; i++) {
            if (dist[i] != LLONG_MAX) {
                potential[i] += dist[i];
            }
        }

        vector<int> path;
        int v = N;
        while (v != 1) {
            path.push_back(v);
            v = prev_node[v];
        }
        path.push_back(1);
        reverse(path.begin(), path.end());
        paths.push_back(path);

        v = N;
        while (v != 1) {
            int u = prev_node[v];
            int idx = prev_edge[v];
            g[u][idx].capacity -= 1;
            g[v][g[u][idx].rev].capacity += 1;
            v = u;
        }
    }

    if (!found || paths.size() < 2) {
        cout << "No solution" << endl;
    } else {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < (int)paths[i].size(); j++) {
                if (j > 0) cout << " ";
                cout << paths[i][j];
            }
            cout << '\n';
        }
    }

    return 0;
}
