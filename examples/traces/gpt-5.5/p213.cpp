#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, S, T;
    cin >> N >> M >> S >> T;
    --S;
    --T;

    vector<pair<int, int>> edges(M);
    vector<vector<int>> adj(N);

    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        --u;
        --v;
        edges[i] = {u, v};
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> dist(N, -1);
    queue<int> q;
    dist[S] = 0;
    q.push(S);

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int to : adj[v]) {
            if (dist[to] == -1) {
                dist[to] = dist[v] + 1;
                q.push(to);
            }
        }
    }

    int L = dist[T];
    vector<vector<int>> answer(L);

    for (int i = 0; i < M; ++i) {
        auto [u, v] = edges[i];

        if (abs(dist[u] - dist[v]) == 1) {
            int layer = min(dist[u], dist[v]);

            if (0 <= layer && layer < L) {
                answer[layer].push_back(i + 1);
            }
        }
    }

    cout << L << '\n';
    for (int i = 0; i < L; ++i) {
        cout << answer[i].size();
        for (int id : answer[i]) {
            cout << ' ' << id;
        }
        cout << '\n';
    }

    return 0;
}
