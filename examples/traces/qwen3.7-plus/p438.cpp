#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <array>

using namespace std;

const int INF = 1e9;

struct Edge {
    int to;
    int cap;
    int flow;
    int rev;
};

struct Dinic {
    int n;
    vector<vector<Edge>> adj;
    vector<int> level;
    vector<int> ptr;

    Dinic(int n) : n(n), adj(n), level(n), ptr(n) {}

    void add_edge(int from, int to, int cap) {
        adj[from].push_back({to, cap, 0, (int)adj[to].size()});
        adj[to].push_back({from, 0, 0, (int)adj[from].size() - 1});
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto& edge : adj[v]) {
                if (edge.cap - edge.flow > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[v] + 1;
                    q.push(edge.to);
                }
            }
        }
        return level[t] != -1;
    }

    int dfs(int v, int t, int pushed) {
        if (pushed == 0) return 0;
        if (v == t) return pushed;
        for (int& cid = ptr[v]; cid < (int)adj[v].size(); ++cid) {
            auto& edge = adj[v][cid];
            int tr = edge.to;
            if (level[v] + 1 != level[tr] || edge.cap - edge.flow == 0) continue;
            int push = dfs(tr, t, min(pushed, edge.cap - edge.flow));
            if (push == 0) continue;
            edge.flow += push;
            adj[tr][edge.rev].flow -= push;
            return push;
        }
        return 0;
    }

    int max_flow(int s, int t, int target) {
        int flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (int pushed = dfs(s, t, INF)) {
                flow += pushed;
                if (flow >= target) return flow;
            }
        }
        return flow;
    }
};

bool possible(int N, long long D, long long W, const vector<array<long long, 3>>& piles) {
    int S = 0, E = 1;
    Dinic dinic(2 + 2 * N);

    for (int i = 1; i <= N; ++i) {
        int u = 2 * i;
        int v = 2 * i + 1;
        dinic.add_edge(u, v, piles[i-1][2]);

        if (piles[i-1][1] <= D) {
            dinic.add_edge(S, u, INF);
        }
        if (W - piles[i-1][1] <= D) {
            dinic.add_edge(v, E, INF);
        }
    }

    for (int i = 1; i <= N; ++i) {
        int v_i = 2 * i + 1;
        for (int j = 1; j <= N; ++j) {
            if (i == j) continue;
            long long dx = piles[i-1][0] - piles[j-1][0];
            long long dy = piles[i-1][1] - piles[j-1][1];
            if (dx * dx + dy * dy <= D * D) {
                int u_j = 2 * j;
                dinic.add_edge(v_i, u_j, INF);
            }
        }
    }

    return dinic.max_flow(S, E, 1) >= 1;
}

bool check(int T, int N, int M, long long D, long long W, const vector<array<long long, 3>>& piles) {
    int S = 0, E = 1;
    int num_nodes = 2 + T * 2 * N;
    Dinic dinic(num_nodes);

    auto get_in = [&](int t, int i) {
        return 2 + (t - 1) * 2 * N + 2 * (i - 1);
    };
    auto get_out = [&](int t, int i) {
        return get_in(t, i) + 1;
    };

    for (int t = 1; t <= T; ++t) {
        for (int i = 1; i <= N; ++i) {
            int u = get_in(t, i);
            int v = get_out(t, i);
            dinic.add_edge(u, v, piles[i-1][2]);

            if (piles[i-1][1] <= D) {
                dinic.add_edge(S, u, INF);
            }

            if (W - piles[i-1][1] <= D) {
                dinic.add_edge(v, E, INF);
            }
        }
    }

    for (int t = 1; t < T; ++t) {
        for (int i = 1; i <= N; ++i) {
            int v_curr = get_out(t, i);
            for (int j = 1; j <= N; ++j) {
                if (i == j) {
                    int u_next = get_in(t + 1, i);
                    dinic.add_edge(v_curr, u_next, INF);
                } else {
                    long long dx = piles[i-1][0] - piles[j-1][0];
                    long long dy = piles[i-1][1] - piles[j-1][1];
                    if (dx * dx + dy * dy <= D * D) {
                        int u_next = get_in(t + 1, j);
                        dinic.add_edge(v_curr, u_next, INF);
                    }
                }
            }
        }
    }

    return dinic.max_flow(S, E, M) >= M;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    long long D, W;
    if (!(cin >> N >> M >> D >> W)) return 0;

    vector<array<long long, 3>> piles(N);
    for (int i = 0; i < N; ++i) {
        cin >> piles[i][0] >> piles[i][1] >> piles[i][2];
    }

    if (W <= D) {
        cout << 1 << "\n";
        return 0;
    }

    if (!possible(N, D, W, piles)) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

    int low = 1, high = 500, ans = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (check(mid, N, M, D, W, piles)) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    if (ans == -1) {
        cout << "IMPOSSIBLE\n";
    } else {
        cout << ans + 1 << "\n";
    }

    return 0;
}
