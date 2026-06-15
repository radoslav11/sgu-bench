#include <bits/stdc++.h>
using namespace std;

static bool diameter_at_most_3(const vector<vector<unsigned char>>& adj) {
    int n = (int)adj.size();
    if (n <= 1) return true;

    vector<int> dist(n);
    queue<int> q;

    for (int s = 0; s < n; ++s) {
        fill(dist.begin(), dist.end(), -1);
        dist[s] = 0;
        q.push(s);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            if (dist[v] == 3) continue;

            for (int to = 0; to < n; ++to) {
                if (adj[v][to] && dist[to] == -1) {
                    dist[to] = dist[v] + 1;
                    q.push(to);
                }
            }
        }

        for (int i = 0; i < n; ++i) {
            if (dist[i] == -1 || dist[i] > 3) return false;
        }
    }

    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    if (N == 1) {
        cout << 0 << "\n\n";
        return 0;
    }

    int K = (M + 1) / 2;

    vector<vector<unsigned char>> g1(N, vector<unsigned char>(N, 0));
    vector<vector<unsigned char>> g2(N, vector<unsigned char>(N, 0));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int c;
            cin >> c;

            if (i == j) continue;

            if (c <= K) g1[i][j] = 1;
            else g2[i][j] = 1;
        }
    }

    if (diameter_at_most_3(g1)) {
        cout << K << '\n';
        for (int i = 1; i <= K; ++i) {
            if (i > 1) cout << ' ';
            cout << i;
        }
        cout << '\n';
    } else if (diameter_at_most_3(g2)) {
        cout << M - K << '\n';
        for (int i = K + 1; i <= M; ++i) {
            if (i > K + 1) cout << ' ';
            cout << i;
        }
        cout << '\n';
    } else {
        cout << -1 << '\n';
    }

    return 0;
}
