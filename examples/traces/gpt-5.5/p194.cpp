#include <bits/stdc++.h>
using namespace std;

struct Dinic {
    struct Edge {
        int to, rev;
        long long cap;
    };

    int n;
    vector<vector<Edge>> g;
    vector<int> level, ptr;

    Dinic(int n) : n(n), g(n), level(n), ptr(n) {}

    int addEdge(int v, int to, long long cap) {
        int idx = (int)g[v].size();
        Edge a{to, (int)g[to].size(), cap};
        Edge b{v, (int)g[v].size(), 0};
        g[v].push_back(a);
        g[to].push_back(b);
        return idx;
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

    long long dfs(int v, int t, long long pushed) {
        if (v == t || pushed == 0) return pushed;

        for (int &cid = ptr[v]; cid < (int)g[v].size(); cid++) {
            Edge &e = g[v][cid];

            if (e.cap > 0 && level[e.to] == level[v] + 1) {
                long long tr = dfs(e.to, t, min(pushed, e.cap));
                if (tr == 0) continue;

                e.cap -= tr;
                g[e.to][e.rev].cap += tr;
                return tr;
            }
        }

        return 0;
    }

    long long maxflow(int s, int t) {
        long long flow = 0;

        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);

            while (true) {
                long long pushed = dfs(s, t, LLONG_MAX / 4);
                if (pushed == 0) break;
                flow += pushed;
            }
        }

        return flow;
    }
};

struct Pipe {
    int u;
    int idx;
    long long lower;
    long long residual_cap;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    int S = 0;
    int T = N + 1;

    Dinic dinic(N + 2);
    vector<long long> balance(N + 1, 0);
    vector<Pipe> pipes;

    for (int k = 0; k < M; k++) {
        int u, v;
        long long l, c;
        cin >> u >> v >> l >> c;

        int idx = dinic.addEdge(u, v, c - l);
        pipes.push_back({u, idx, l, c - l});

        balance[u] -= l;
        balance[v] += l;
    }

    long long need = 0;

    for (int i = 1; i <= N; i++) {
        if (balance[i] > 0) {
            dinic.addEdge(S, i, balance[i]);
            need += balance[i];
        } else if (balance[i] < 0) {
            dinic.addEdge(i, T, -balance[i]);
        }
    }

    long long got = dinic.maxflow(S, T);

    if (got != need) {
        cout << "NO\n";
        return 0;
    }

    cout << "YES\n";

    for (const Pipe &p : pipes) {
        long long used = p.residual_cap - dinic.g[p.u][p.idx].cap;
        cout << p.lower + used << '\n';
    }

    return 0;
}
