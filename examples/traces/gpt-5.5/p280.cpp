#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;

    vector<vector<int>> g(N + 1);
    for (int i = 0; i < N - 1; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    vector<int> parent(N + 1, 0), depth(N + 1, 0), order;
    order.reserve(N);

    queue<int> q;
    q.push(1);
    parent[1] = 0;

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        order.push_back(v);

        for (int to : g[v]) {
            if (to == parent[v]) continue;
            parent[to] = v;
            depth[to] = depth[v] + 1;
            q.push(to);
        }
    }

    vector<char> covered(N + 1, 0);
    vector<int> centers;

    auto markCovered = [&](int start) {
        queue<tuple<int, int, int>> bfs;
        bfs.push({start, 0, 0});

        while (!bfs.empty()) {
            auto [v, p, dist] = bfs.front();
            bfs.pop();

            covered[v] = 1;

            if (dist == K) continue;

            for (int to : g[v]) {
                if (to == p) continue;
                bfs.push({to, v, dist + 1});
            }
        }
    };

    for (int i = (int)order.size() - 1; i >= 0; --i) {
        int v = order[i];

        if (covered[v]) continue;

        int center = v;
        for (int step = 0; step < K && parent[center] != 0; ++step) {
            center = parent[center];
        }

        centers.push_back(center);
        markCovered(center);
    }

    cout << centers.size() << '\n';
    for (int i = 0; i < (int)centers.size(); ++i) {
        if (i) cout << ' ';
        cout << centers[i];
    }
    cout << '\n';

    return 0;
}
