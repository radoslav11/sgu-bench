#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<vector<pair<int, int>>> graph(N + 1);

    for (int i = 0; i < M; ++i) {
        int X, Y, C;
        cin >> X >> Y >> C;
        graph[X].push_back({Y, C});
    }

    const int INF = 1e9;
    vector<vector<int>> dist(N + 1, vector<int>(4, INF));

    queue<pair<int, int>> q;

    dist[1][0] = 0;
    q.push({1, 0});

    while (!q.empty()) {
        auto [v, lastColor] = q.front();
        q.pop();

        for (auto [to, color] : graph[v]) {
            if (color == lastColor) continue;

            if (dist[to][color] > dist[v][lastColor] + 1) {
                dist[to][color] = dist[v][lastColor] + 1;
                q.push({to, color});
            }
        }
    }

    int answer = min({dist[N][1], dist[N][2], dist[N][3]});

    if (answer == INF) {
        cout << -1 << '\n';
    } else {
        cout << answer << '\n';
    }

    return 0;
}
