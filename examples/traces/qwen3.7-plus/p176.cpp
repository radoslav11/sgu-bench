#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Edge {
    int to;
    long long cap;
    long long flow;
    int rev;
};

vector<vector<Edge>> adj;
vector<int> level;
vector<int> ptr;

int add_edge_with_idx_and_rev_cap(int from, int to, long long cap_fwd, long long cap_rev) {
    int idx = adj[from].size();
    adj[from].push_back({to, cap_fwd, 0, (int)adj[to].size()});
    adj[to].push_back({from, cap_rev, 0, idx});
    return idx;
}

void add_edge(int from, int to, long long cap) {
    add_edge_with_idx_and_rev_cap(from, to, cap, 0);
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

long long dfs(int v, int t, long long pushed) {
    if (pushed == 0) return 0;
    if (v == t) return pushed;
    for (int& cid = ptr[v]; cid < (int)adj[v].size(); ++cid) {
        auto& edge = adj[v][cid];
        int tr = edge.to;
        if (level[v] + 1 != level[tr] || edge.cap - edge.flow == 0) continue;
        long long push = dfs(tr, t, min(pushed, edge.cap - edge.flow));
        if (push == 0) continue;
        edge.flow += push;
        adj[tr][edge.rev].flow -= push;
        return push;
    }
    return 0;
}

long long dinic(int s, int t) {
    long long flow = 0;
    while (bfs(s, t)) {
        fill(ptr.begin(), ptr.end(), 0);
        while (long long pushed = dfs(s, t, 1e18)) {
            flow += pushed;
        }
    }
    return flow;
}

struct EdgeInfo {
    int u, v;
    long long L, U;
    int edge_idx;
    int orig_idx;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    int S = 0, T = N + 1;
    adj.assign(N + 2, vector<Edge>());
    level.assign(N + 2, 0);
    ptr.assign(N + 2, 0);

    vector<long long> D(N + 2, 0);
    vector<EdgeInfo> orig_edges;

    for (int i = 0; i < M; ++i) {
        int u, v;
        long long z, c;
        cin >> u >> v >> z >> c;
        long long L = (c == 1) ? z : 0;
        long long U = z;
        D[v] += L;
        D[u] -= L;
        int idx = add_edge_with_idx_and_rev_cap(u, v, U - L, 0);
        orig_edges.push_back({u, v, L, U, idx, i});
    }

    add_edge(N, 1, 1e18);

    long long sum_D = 0;
    for (int i = 1; i <= N; ++i) {
        if (D[i] > 0) {
            add_edge(S, i, D[i]);
            sum_D += D[i];
        } else if (D[i] < 0) {
            add_edge(i, T, -D[i]);
        }
    }

    long long max_flow_1 = dinic(S, T);
    if (max_flow_1 < sum_D) {
        cout << "Impossible\n";
        return 0;
    }

    vector<long long> ans_flow(M);
    for (int i = 0; i < M; ++i) {
        ans_flow[i] = orig_edges[i].L + adj[orig_edges[i].u][orig_edges[i].edge_idx].flow;
    }

    long long f_ret = 0;
    for (int i = 0; i < M; ++i) {
        if (orig_edges[i].u == 1) f_ret += ans_flow[i];
        if (orig_edges[i].v == 1) f_ret -= ans_flow[i];
    }

    adj.assign(N + 3, vector<Edge>());
    level.assign(N + 3, 0);
    ptr.assign(N + 3, 0);

    vector<int> new_idx(M);
    for (int i = 0; i < M; ++i) {
        long long f = ans_flow[i];
        long long L = orig_edges[i].L;
        long long U = orig_edges[i].U;
        int u = orig_edges[i].u;
        int v = orig_edges[i].v;
        new_idx[i] = add_edge_with_idx_and_rev_cap(u, v, U - f, f - L);
    }

    int super_source = N + 2;
    add_edge_with_idx_and_rev_cap(super_source, N, f_ret, 0);

    long long f_pushed = dinic(super_source, 1);

    for (int i = 0; i < M; ++i) {
        int u = orig_edges[i].u;
        int idx = new_idx[i];
        ans_flow[i] += adj[u][idx].flow;
    }

    cout << f_ret - f_pushed << "\n";
    for (int i = 0; i < M; ++i) {
        cout << ans_flow[i] << (i == M - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}
