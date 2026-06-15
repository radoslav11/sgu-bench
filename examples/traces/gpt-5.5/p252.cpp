#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to, rev, cap, cost;
};

struct MinCostMaxFlow {
    int n;
    vector<vector<Edge>> g;

    MinCostMaxFlow(int n) : n(n), g(n) {}

    int addEdge(int v, int to, int cap, int cost) {
        Edge a{to, (int)g[to].size(), cap, cost};
        Edge b{v, (int)g[v].size(), 0, -cost};
        g[v].push_back(a);
        g[to].push_back(b);
        return (int)g[v].size() - 1;
    }

    pair<int, int> minCostMaxFlow(int s, int t) {
        int flow = 0, cost = 0;

        while (true) {
            const int INF = 1e9;
            vector<int> dist(n, INF), pv(n, -1), pe(n, -1);
            vector<bool> inq(n, false);
            queue<int> q;

            dist[s] = 0;
            q.push(s);
            inq[s] = true;

            while (!q.empty()) {
                int v = q.front();
                q.pop();
                inq[v] = false;

                for (int i = 0; i < (int)g[v].size(); ++i) {
                    Edge &e = g[v][i];
                    if (e.cap > 0 && dist[e.to] > dist[v] + e.cost) {
                        dist[e.to] = dist[v] + e.cost;
                        pv[e.to] = v;
                        pe[e.to] = i;
                        if (!inq[e.to]) {
                            inq[e.to] = true;
                            q.push(e.to);
                        }
                    }
                }
            }

            if (dist[t] == INF) break;

            int add = 1;
            for (int v = t; v != s; v = pv[v]) {
                add = min(add, g[pv[v]][pe[v]].cap);
            }

            for (int v = t; v != s; v = pv[v]) {
                Edge &e = g[pv[v]][pe[v]];
                e.cap -= add;
                g[v][e.rev].cap += add;
                cost += add * e.cost;
            }

            flow += add;
        }

        return {flow, cost};
    }
};

struct RoadRef {
    int a, b;
    int node, edgeIndex;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    int S = 0;
    int L = 1;
    int R = 1 + N;
    int T = 1 + 2 * N;

    MinCostMaxFlow mf(T + 1);

    for (int i = 1; i <= N; ++i) {
        mf.addEdge(S, L + i - 1, 1, 0);
        mf.addEdge(R + i - 1, T, 1, 0);
    }

    vector<RoadRef> roads;

    for (int i = 0; i < M; ++i) {
        int a, b, c;
        cin >> a >> b >> c;

        int u = L + a - 1;
        int v = R + b - 1;
        int idx = mf.addEdge(u, v, 1, c);

        roads.push_back({a, b, u, idx});
    }

    auto [matchingSize, totalCost] = mf.minCostMaxFlow(S, T);

    vector<int> succ(N + 1, 0), pred(N + 1, 0);

    for (auto &r : roads) {
        Edge &e = mf.g[r.node][r.edgeIndex];
        if (e.cap == 0) {
            succ[r.a] = r.b;
            pred[r.b] = r.a;
        }
    }

    vector<vector<int>> paths;

    for (int i = 1; i <= N; ++i) {
        if (pred[i] == 0) {
            vector<int> path;
            int v = i;
            while (v != 0) {
                path.push_back(v);
                v = succ[v];
            }
            paths.push_back(path);
        }
    }

    cout << paths.size() << ' ' << totalCost << '\n';

    for (auto &path : paths) {
        cout << path.size();
        for (int v : path) {
            cout << ' ' << v;
        }
        cout << '\n';
    }

    return 0;
}
