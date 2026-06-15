#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to;
    int w;
};

const long long INF = (1LL << 62);

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    int s, t;
    cin >> s >> t;
    --s;
    --t;

    vector<vector<Edge>> g(n), rg(n);

    for (int i = 0; i < m; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        --a;
        --b;
        g[a].push_back({b, c});
        rg[b].push_back({a, c});
    }

    vector<long long> h(n, INF);

    {
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        h[t] = 0;
        pq.push({0, t});

        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();

            if (d != h[v]) continue;

            for (const Edge &e : rg[v]) {
                if (h[e.to] > d + e.w) {
                    h[e.to] = d + e.w;
                    pq.push({h[e.to], e.to});
                }
            }
        }
    }

    if (h[s] == INF) {
        for (int i = 0; i < k; ++i) {
            cout << "NO\n";
        }
        return 0;
    }

    struct State {
        long long f;
        long long dist;
        int v;

        bool operator<(const State &other) const {
            if (f != other.f) return f > other.f;
            return dist > other.dist;
        }
    };

    priority_queue<State> pq;
    vector<int> used(n, 0);
    vector<long long> answer;

    pq.push({h[s], 0, s});

    while (!pq.empty() && (int)answer.size() < k) {
        State cur = pq.top();
        pq.pop();

        int v = cur.v;

        if (used[v] >= k) continue;
        ++used[v];

        if (v == t) {
            answer.push_back(cur.dist);
        }

        if ((int)answer.size() == k) break;

        for (const Edge &e : g[v]) {
            if (h[e.to] == INF) continue;

            long long nd = cur.dist + e.w;
            pq.push({nd + h[e.to], nd, e.to});
        }
    }

    for (int i = 0; i < k; ++i) {
        if (i < (int)answer.size()) cout << answer[i] << '\n';
        else cout << "NO\n";
    }

    return 0;
}
