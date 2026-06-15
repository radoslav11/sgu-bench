#include <bits/stdc++.h>
using namespace std;

struct Dinic {
    struct Edge {
        int to, rev, cap;
    };

    int n;
    vector<vector<Edge>> g;
    vector<int> level, it;

    Dinic(int n) : n(n), g(n), level(n), it(n) {}

    void addEdge(int v, int to, int cap) {
        Edge a{to, (int)g[to].size(), cap};
        Edge b{v, (int)g[v].size(), 0};
        g[v].push_back(a);
        g[to].push_back(b);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (const Edge &e : g[v]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }

        return level[t] != -1;
    }

    int dfs(int v, int t, int pushed) {
        if (v == t || pushed == 0) return pushed;

        for (int &i = it[v]; i < (int)g[v].size(); i++) {
            Edge &e = g[v][i];

            if (e.cap > 0 && level[e.to] == level[v] + 1) {
                int tr = dfs(e.to, t, min(pushed, e.cap));
                if (tr > 0) {
                    e.cap -= tr;
                    g[e.to][e.rev].cap += tr;
                    return tr;
                }
            }
        }

        return 0;
    }

    int maxflow(int s, int t) {
        int flow = 0;

        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);

            while (true) {
                int pushed = dfs(s, t, INT_MAX);
                if (pushed == 0) break;
                flow += pushed;
            }
        }

        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<int> w(N), r(N);
    for (int i = 0; i < N; i++) cin >> w[i];
    for (int i = 0; i < N; i++) cin >> r[i];

    vector<vector<int>> a(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> a[i][j];
        }
    }

    int target = w[0] + r[0];

    for (int i = 1; i < N; i++) {
        if (w[i] > target) {
            cout << "NO\n";
            return 0;
        }
    }

    vector<pair<int, int>> games;
    int totalGames = 0;

    for (int i = 1; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (a[i][j] > 0) {
                games.push_back({i, j});
                totalGames += a[i][j];
            }
        }
    }

    int gameCount = games.size();
    int source = 0;
    int gameStart = 1;
    int teamStart = gameStart + gameCount;
    int sink = teamStart + (N - 1);
    int nodes = sink + 1;

    Dinic dinic(nodes);

    for (int k = 0; k < gameCount; k++) {
        int i = games[k].first;
        int j = games[k].second;
        int gameNode = gameStart + k;

        dinic.addEdge(source, gameNode, a[i][j]);
        dinic.addEdge(gameNode, teamStart + (i - 1), INT_MAX / 2);
        dinic.addEdge(gameNode, teamStart + (j - 1), INT_MAX / 2);
    }

    for (int i = 1; i < N; i++) {
        int cap = target - w[i];
        dinic.addEdge(teamStart + (i - 1), sink, cap);
    }

    int flow = dinic.maxflow(source, sink);

    cout << (flow == totalGames ? "YES" : "NO") << '\n';

    return 0;
}
