#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N, K;
    cin >> N >> K;

    vector<vector<int>> graph(N + 1);
    for (int i = 0; i < N - 1; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    vector<int> depth(N + 1, -1);
    vector<int> parent(N + 1, 0);

    queue<int> q;
    depth[1] = 0;
    q.push(1);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : graph[u]) {
            if (depth[v] == -1) {
                depth[v] = depth[u] + 1;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    vector<int> nodes(N);
    for (int i = 0; i < N; i++) {
        nodes[i] = i + 1;
    }
    sort(nodes.begin(), nodes.end(), [&](int a, int b) {
        return depth[a] > depth[b];
    });

    const int INF = 1000000;
    vector<int> cover(N + 1, INF);
    vector<int> centers;

    for (int u : nodes) {
        if (cover[u] > K) {
            int v = u;
            int steps = K;
            while (steps > 0 && parent[v] != 0) {
                v = parent[v];
                steps--;
            }
            centers.push_back(v);

            vector<bool> visited(N + 1, false);
            queue<pair<int, int>> qq;
            qq.push({v, 0});
            visited[v] = true;
            while (!qq.empty()) {
                int cur = qq.front().first;
                int d = qq.front().second;
                qq.pop();
                cover[cur] = min(cover[cur], d);
                if (d < K) {
                    for (int neighbor : graph[cur]) {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            qq.push({neighbor, d + 1});
                        }
                    }
                }
            }
        }
    }

    cout << centers.size() << "\n";
    for (int c : centers) {
        cout << c << "\n";
    }

    return 0;
}
