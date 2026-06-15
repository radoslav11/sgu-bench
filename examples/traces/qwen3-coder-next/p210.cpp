#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <climits>

using namespace std;

struct Edge {
    int to, cap, cost, rev;
    Edge(int t, int c, int co, int r) : to(t), cap(c), cost(co), rev(r) {}
};

struct MinCostFlow {
    int n;
    vector<vector<Edge>> graph;
    vector<int> dist, prevv, preve;
    const int INF = INT_MAX / 2;

    MinCostFlow(int size) : n(size), graph(size), dist(size), prevv(size), preve(size) {}

    void addEdge(int from, int to, int cap, int cost) {
        graph[from].emplace_back(to, cap, cost, graph[to].size());
        graph[to].emplace_back(from, 0, -cost, graph[from].size() - 1);
    }

    pair<int, int> minCostFlow(int s, int t, int f) {
        int res = 0;
        while (f > 0) {
            fill(dist.begin(), dist.end(), INF);
            dist[s] = 0;
            bool update = true;
            while (update) {
                update = false;
                for (int v = 0; v < n; ++v) {
                    if (dist[v] == INF) continue;
                    for (int i = 0; i < graph[v].size(); ++i) {
                        Edge &e = graph[v][i];
                        if (e.cap > 0 && dist[e.to] > dist[v] + e.cost) {
                            dist[e.to] = dist[v] + e.cost;
                            prevv[e.to] = v;
                            preve[e.to] = i;
                            update = true;
                        }
                    }
                }
            }
            if (dist[t] == INF) {
                return {res, f};
            }
            int d = f;
            for (int v = t; v != s; v = prevv[v]) {
                d = min(d, graph[prevv[v]][preve[v]].cap);
            }
            f -= d;
            res += d * dist[t];
            for (int v = t; v != s; v = prevv[v]) {
                Edge &e = graph[prevv[v]][preve[v]];
                e.cap -= d;
                graph[v][e.rev].cap += d;
            }
        }
        return {res, 0};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    vector<vector<int>> preferences(n);
    for (int i = 0; i < n; ++i) {
        int k;
        cin >> k;
        preferences[i].resize(k);
        for (int j = 0; j < k; ++j) {
            cin >> preferences[i][j];
            preferences[i][j]--; // Convert to 0-indexed
        }
    }

    // Build graph for maximum weight matching
    // We want to maximize sqrt(sum_{i in L} a[i]^2) which is equivalent to maximizing sum_{i in L} a[i]^2
    // So we can use maximum weight bipartite matching where weight for son i choosing girl j is a[i]^2 if j is in his preferences, else 0
    // But we want exactly one girl per son (at most one) and one son per girl (exactly one if possible, but girls not required to be used)

    // We'll create a bipartite graph: left side = sons (0 to n-1), right side = girls (0 to n-1)
    // Add edge from son i to girl j with weight = a[i]^2 if j is in preferences[i]
    // We want to find matching that maximizes sum of weights

    // For minimum cost flow, we can set cost = -weight and minimize
    // But note: weights are positive, so negative weights work

    // Total nodes: source (0), sons (1..n), girls (n+1..2n), sink (2n+1) => 2n+2 nodes
    int nodes = 2 * n + 2;
    int source = 0, sink = 2 * n + 1;
    MinCostFlow mcf(nodes);

    // Connect source to each son with capacity 1, cost 0
    for (int i = 0; i < n; ++i) {
        mcf.addEdge(source, i + 1, 1, 0);
    }

    // Connect each son to his liked girls with capacity 1, cost = -a[i]^2
    for (int i = 0; i < n; ++i) {
        long long weight = (long long)a[i] * a[i];
        for (int girl : preferences[i]) {
            mcf.addEdge(i + 1, n + 1 + girl, 1, -(int)weight);
        }
    }

    // Connect each girl to sink with capacity 1, cost 0
    for (int j = 0; j < n; ++j) {
        mcf.addEdge(n + 1 + j, sink, 1, 0);
    }

    // Run min cost max flow to get maximum weight matching
    auto [flow, f] = mcf.minCostFlow(source, sink, n);

    // Now reconstruct the matching
    vector<int> result(n, 0);
    for (int i = 0; i < n; ++i) {
        for (const Edge &e : mcf.graph[i + 1]) {
            if (e.to >= n + 1 && e.to <= 2 * n && e.cap == 0 && e.cost < 0) {
                // This edge is used (flow passed through)
                int girl_index = e.to - (n + 1);
                result[i] = girl_index + 1; // Convert back to 1-indexed
                break;
            }
        }
    }

    // Output the result
    for (int i = 0; i < n; ++i) {
        if (i > 0) cout << " ";
        cout << result[i];
    }
    cout << "\n";

    return 0;
}
