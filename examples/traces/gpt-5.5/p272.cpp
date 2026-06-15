#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<vector<int>> g(N + 1);
    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    int N1;
    cin >> N1;
    vector<int> A(N1);
    vector<char> isA(N + 1, false);
    for (int i = 0; i < N1; ++i) {
        cin >> A[i];
        isA[A[i]] = true;
    }

    int N2;
    cin >> N2;
    vector<int> B(N2);
    vector<char> isB(N + 1, false);
    for (int i = 0; i < N2; ++i) {
        cin >> B[i];
        isB[B[i]] = true;
    }

    const int INF = 1e9;

    auto bfs = [&](const vector<int>& starts) {
        vector<int> dist(N + 1, INF);
        queue<int> q;

        for (int s : starts) {
            dist[s] = 0;
            q.push(s);
        }

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int to : g[v]) {
                if (dist[to] == INF) {
                    dist[to] = dist[v] + 1;
                    q.push(to);
                }
            }
        }

        return dist;
    };

    vector<int> da = bfs(A);
    vector<int> db = bfs(B);

    int K = INF;
    for (int a : A) {
        K = min(K, db[a]);
    }

    vector<char> valid(N + 1, false);
    for (int v = 1; v <= N; ++v) {
        if (da[v] != INF && db[v] != INF && da[v] + db[v] == K) {
            valid[v] = true;
        }
    }

    vector<char> used(N + 1, false), dead(N + 1, false);
    vector<vector<int>> answer;
    vector<int> path;

    function<bool(int)> dfs = [&](int v) -> bool {
        if (used[v] || dead[v] || !valid[v]) return false;

        if (da[v] == K) {
            path.push_back(v);
            return true;
        }

        for (int to : g[v]) {
            if (valid[to] && da[to] == da[v] + 1) {
                if (dfs(to)) {
                    path.push_back(v);
                    return true;
                }
            }
        }

        dead[v] = true;
        return false;
    };

    for (int s : A) {
        if (!valid[s] || used[s] || dead[s]) continue;

        path.clear();

        if (dfs(s)) {
            reverse(path.begin(), path.end());

            for (int v : path) {
                used[v] = true;
            }

            answer.push_back(path);
        }
    }

    cout << answer.size() << ' ' << K << '\n';
    for (const auto& p : answer) {
        for (int i = 0; i < (int)p.size(); ++i) {
            if (i) cout << ' ';
            cout << p[i];
        }
        cout << '\n';
    }

    return 0;
}
