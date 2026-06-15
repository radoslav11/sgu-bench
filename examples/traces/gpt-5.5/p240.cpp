#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to;
    long long t, r, p;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, S, E;
    long long H;
    cin >> N >> M >> H >> S >> E;

    vector<vector<Edge>> g(N + 1);

    for (int i = 0; i < M; ++i) {
        int A, B;
        long long T, R, P;
        cin >> A >> B >> T >> R >> P;
        g[A].push_back({B, T, R, P});
        g[B].push_back({A, T, R, P});
    }

    vector<int> exits(E);
    vector<int> isExit(N + 1, 0);
    for (int i = 0; i < E; ++i) {
        cin >> exits[i];
        isExit[exits[i]] = 1;
    }

    const long long INF = (1LL << 60);

    auto dijkstra = [&](long long limit, vector<int>* parent = nullptr) -> int {
        vector<long long> dist(N + 1, INF);
        vector<int> par(N + 1, -1);

        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

        dist[S] = 0;
        pq.push({0, S});

        while (!pq.empty()) {
            auto [curTime, v] = pq.top();
            pq.pop();

            if (curTime != dist[v]) continue;

            if (isExit[v]) {
                if (parent) *parent = par;
                return v;
            }

            for (const Edge& e : g[v]) {
                long long newTime = curTime + e.t;

                bool can = false;
                if (e.p == 0) {
                    can = (e.r <= limit);
                } else {
                    can = (e.r + e.p * newTime <= limit);
                }

                if (!can) continue;

                if (newTime < dist[e.to]) {
                    dist[e.to] = newTime;
                    par[e.to] = v;
                    pq.push({newTime, e.to});
                }
            }
        }

        if (parent) *parent = par;
        return -1;
    };

    if (dijkstra(H) == -1) {
        cout << "NO\n";
        return 0;
    }

    long long lo = 0, hi = H;
    while (lo < hi) {
        long long mid = (lo + hi) / 2;
        if (dijkstra(mid) != -1) hi = mid;
        else lo = mid + 1;
    }

    vector<int> parent;
    int finish = dijkstra(lo, &parent);

    vector<int> path;
    int v = finish;
    while (v != -1) {
        path.push_back(v);
        if (v == S) break;
        v = parent[v];
    }
    reverse(path.begin(), path.end());

    cout << "YES\n";
    cout << lo << '\n';
    cout << path.size();
    for (int x : path) cout << '73' << x;
    cout << '\n';

    return 0;
}
