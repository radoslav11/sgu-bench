#include <bits/stdc++.h>
using namespace std;

struct Dinic {
    struct Edge {
        int to, rev, cap;
    };

    int n;
    vector<vector<Edge>> g;
    vector<int> level, it;

    Dinic(int n = 0) : n(n), g(n), level(n), it(n) {}

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

            for (auto &e : g[v]) {
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
                if (tr) {
                    e.cap -= tr;
                    g[e.to][e.rev].cap += tr;
                    return tr;
                }
            }
        }

        return 0;
    }

    void maxflow(int s, int t) {
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (dfs(s, t, INT_MAX)) {}
        }
    }

    vector<int> reachable(int s) {
        vector<int> used(n, 0);
        queue<int> q;
        used[s] = 1;
        q.push(s);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto &e : g[v]) {
                if (e.cap > 0 && !used[e.to]) {
                    used[e.to] = 1;
                    q.push(e.to);
                }
            }
        }

        return used;
    }
};

struct Road {
    int a, b, c;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<Road> roads(M);
    for (int i = 0; i < M; i++) {
        cin >> roads[i].a >> roads[i].b >> roads[i].c;
        --roads[i].a;
        --roads[i].b;
    }

    vector<vector<pair<int, int>>> tree(N);
    for (int i = 0; i < N - 1; i++) {
        tree[roads[i].a].push_back({roads[i].b, i});
        tree[roads[i].b].push_back({roads[i].a, i});
    }

    vector<pair<int, int>> constraints;

    for (int e = N - 1; e < M; e++) {
        int s = roads[e].a;
        int target = roads[e].b;

        vector<int> parent(N, -1), parentEdge(N, -1);
        queue<int> q;
        q.push(s);
        parent[s] = s;

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            if (v == target) break;

            for (auto [to, id] : tree[v]) {
                if (parent[to] == -1) {
                    parent[to] = v;
                    parentEdge[to] = id;
                    q.push(to);
                }
            }
        }

        int v = target;
        while (v != s) {
            int tEdge = parentEdge[v];
            constraints.push_back({tEdge, e});
            v = parent[v];
        }
    }

    int maxC = 0;
    vector<int> starts;
    starts.push_back(1);

    for (auto &r : roads) {
        maxC = max(maxC, r.c);
    }

    for (auto &r : roads) {
        if (r.c + 1 <= maxC) {
            starts.push_back(r.c + 1);
        }
    }

    sort(starts.begin(), starts.end());
    starts.erase(unique(starts.begin(), starts.end()), starts.end());

    vector<int> answer(M, 0);

    const int B = M + 1;
    const int INF = 1000000000;

    for (int idx = 0; idx < (int)starts.size(); idx++) {
        int k = starts[idx];
        int nextK = (idx + 1 < (int)starts.size() ? starts[idx + 1] : maxC + 1);
        int len = nextK - k;

        int S = M;
        int T = M + 1;

        Dinic dinic(M + 2);

        for (int i = 0; i < M; i++) {
            int profit = (roads[i].c >= k ? 1 : -1);
            int scaledProfit = profit * B + 1;

            if (scaledProfit >= 0) {
                dinic.addEdge(S, i, scaledProfit);
            } else {
                dinic.addEdge(i, T, -scaledProfit);
            }
        }

        for (auto [u, v] : constraints) {
            dinic.addEdge(u, v, INF);
        }

        dinic.maxflow(S, T);
        vector<int> reach = dinic.reachable(S);

        for (int i = 0; i < M; i++) {
            if (reach[i]) {
                answer[i] += len;
            }
        }
    }

    for (int i = 0; i < M; i++) {
        cout << answer[i] << '\n';
    }

    return 0;
}
