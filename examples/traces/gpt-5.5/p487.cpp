#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int a, b;
    int ga, gb;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    int M = 2 * N;
    vector<Edge> edges(M);
    vector<vector<pair<int,int>>> adj(N + 1);

    for (int i = 0; i < M; i++) {
        int x, y;
        cin >> x >> y;

        int tx = (x - 1) / 4 + 1;
        int ty = (y - 1) / 4 + 1;

        edges[i] = {tx, ty, x, y};

        adj[tx].push_back({ty, i});
        adj[ty].push_back({tx, i});
    }

    vector<int> visited(N + 1, 0);
    queue<int> q;
    q.push(1);
    visited[1] = 1;

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (auto [to, id] : adj[v]) {
            if (!visited[to]) {
                visited[to] = 1;
                q.push(to);
            }
        }
    }

    for (int i = 1; i <= N; i++) {
        if (!visited[i]) {
            cout << "No\n";
            return 0;
        }
    }

    vector<int> used(M, 0);
    vector<int> ptr(N + 1, 0);
    vector<tuple<int,int,int>> tour;

    function<void(int)> dfs = [&](int v) {
        while (ptr[v] < (int)adj[v].size()) {
            auto [to, id] = adj[v][ptr[v]++];
            if (used[id]) continue;

            used[id] = 1;
            dfs(to);
            tour.push_back({v, to, id});
        }
    };

    dfs(1);

    if ((int)tour.size() != M) {
        cout << "No\n";
        return 0;
    }

    reverse(tour.begin(), tour.end());

    vector<int> answer;
    answer.reserve(4 * N);

    for (auto [from, to, id] : tour) {
        const Edge &e = edges[id];

        if (from == e.a) {
            answer.push_back(e.ga);
            answer.push_back(e.gb);
        } else {
            answer.push_back(e.gb);
            answer.push_back(e.ga);
        }
    }

    cout << "Yes\n";
    for (int i = 0; i < (int)answer.size(); i++) {
        if (i) cout << ' ';
        cout << answer[i];
    }
    cout << '\n';

    return 0;
}
