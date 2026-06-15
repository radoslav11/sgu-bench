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

    int dfs(int v, int t, int f) {
        if (v == t) return f;

        for (int &i = it[v]; i < (int)g[v].size(); i++) {
            Edge &e = g[v][i];

            if (e.cap > 0 && level[e.to] == level[v] + 1) {
                int ret = dfs(e.to, t, min(f, e.cap));
                if (ret > 0) {
                    e.cap -= ret;
                    g[e.to][e.rev].cap += ret;
                    return ret;
                }
            }
        }

        return 0;
    }

    int maxflow(int s, int t) {
        int flow = 0;
        const int INF = 1e9;

        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);

            while (true) {
                int pushed = dfs(s, t, INF);
                if (!pushed) break;
                flow += pushed;
            }
        }

        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, D, W;
    cin >> N >> M >> D >> W;

    vector<int> x(N + 2), y(N + 2), cap(N + 2);

    const int START = 0;
    const int FINISH = N + 1;
    const int INF = 1e8;

    y[START] = 0;
    y[FINISH] = W;
    cap[START] = cap[FINISH] = INF;

    for (int i = 1; i <= N; i++) {
        cin >> x[i] >> y[i] >> cap[i];
    }

    int K = N + 2;

    vector<vector<bool>> can(K, vector<bool>(K, false));

    auto jumpPossible = [&](int a, int b) -> bool {
        if (a == b) return true;

        if ((a == START && b == FINISH) || (a == FINISH && b == START)) {
            return W <= D;
        }

        if (a == START) return y[b] <= D;
        if (b == START) return y[a] <= D;

        if (a == FINISH) return W - y[b] <= D;
        if (b == FINISH) return W - y[a] <= D;

        long long dx = x[a] - x[b];
        long long dy = y[a] - y[b];
        return dx * dx + dy * dy <= 1LL * D * D;
    };

    for (int i = 0; i < K; i++) {
        for (int j = 0; j < K; j++) {
            can[i][j] = jumpPossible(i, j);
        }
    }

    vector<int> vis(K, 0);
    queue<int> q;
    vis[START] = 1;
    q.push(START);

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int to = 0; to < K; to++) {
            if (vis[to] || !can[v][to]) continue;
            if (to != FINISH && cap[to] == 0) continue;

            vis[to] = 1;
            q.push(to);
        }
    }

    if (!vis[FINISH]) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

    auto feasible = [&](int T) -> bool {
        int nodesPerTime = K * 2;
        int S = nodesPerTime * (T + 1);
        int TT = S + 1;

        Dinic din(TT + 1);

        auto inNode = [&](int t, int v) {
            return t * nodesPerTime + v * 2;
        };

        auto outNode = [&](int t, int v) {
            return t * nodesPerTime + v * 2 + 1;
        };

        for (int t = 0; t <= T; t++) {
            for (int v = 0; v < K; v++) {
                din.addEdge(inNode(t, v), outNode(t, v), min(cap[v], INF));
            }
        }

        din.addEdge(S, inNode(0, START), M);

        for (int t = 0; t <= T; t++) {
            din.addEdge(outNode(t, FINISH), TT, INF);
        }

        for (int t = 0; t < T; t++) {
            for (int v = 0; v < K; v++) {
                for (int to = 0; to < K; to++) {
                    if (can[v][to]) {
                        din.addEdge(outNode(t, v), inNode(t + 1, to), INF);
                    }
                }
            }
        }

        return din.maxflow(S, TT) >= M;
    };

    int lo = 0, hi = N + M + 2;

    while (!feasible(hi)) {
        hi *= 2;
    }

    while (lo + 1 < hi) {
        int mid = (lo + hi) / 2;

        if (feasible(mid)) hi = mid;
        else lo = mid;
    }

    cout << hi << '\n';

    return 0;
}
