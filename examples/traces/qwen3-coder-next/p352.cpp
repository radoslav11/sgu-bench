#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <algorithm>
#include <climits>
using namespace std;

const long long INF = 1e18;

struct SegmentTree {
    int n;
    vector<long long> seg;
    vector<long long> lazy;
    SegmentTree(int size) {
        n = size;
        seg.resize(4 * n, INF);
        lazy.resize(4 * n, INF);
    }
    void push(int node) {
        if (lazy[node] == INF) return;
        seg[node] = min(seg[node], lazy[node]);
        if (node * 2 < 4 * n) {
            lazy[node * 2] = min(lazy[node * 2], lazy[node]);
            lazy[node * 2 + 1] = min(lazy[node * 2 + 1], lazy[node]);
        }
        lazy[node] = INF;
    }
    void update(int node, int l, int r, int ql, int qr, long long w) {
        push(node);
        if (ql > qr) return;
        if (ql <= l && r <= qr) {
            lazy[node] = min(lazy[node], w);
            push(node);
            return;
        }
        int mid = (l + r) / 2;
        if (ql <= mid) {
            update(node * 2, l, mid, ql, qr, w);
        }
        if (qr > mid) {
            update(node * 2 + 1, mid + 1, r, ql, qr, w);
        }
    }
    long long query(int node, int l, int r, int idx) {
        push(node);
        if (l == r) {
            return seg[node];
        }
        int mid = (l + r) / 2;
        if (idx <= mid) {
            return query(node * 2, l, mid, idx);
        } else {
            return query(node * 2 + 1, mid + 1, r, idx);
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N, M;
    cin >> N >> M;

    vector<vector<pair<int, long long>>> adjT_weighted(N + 1);
    vector<tuple<int, int, int>> non_tree_edges;

    for (int i = 0; i < M; i++) {
        int a, b, l, t;
        cin >> a >> b >> l >> t;
        if (t == 1) {
            adjT_weighted[a].push_back({b, l});
            adjT_weighted[b].push_back({a, l});
        } else {
            non_tree_edges.push_back({a, b, l});
        }
    }

    vector<long long> base_dist(N + 1, INF);
    vector<int> parent0(N + 1, 0);
    vector<int> depth_vec(N + 1, -1);
    queue<int> q;
    base_dist[1] = 0;
    depth_vec[1] = 0;
    parent0[1] = 0;
    q.push(1);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto [v, w] : adjT_weighted[u]) {
            if (v == parent0[u]) continue;
            parent0[v] = u;
            base_dist[v] = base_dist[u] + w;
            depth_vec[v] = depth_vec[u] + 1;
            q.push(v);
        }
    }

    vector<vector<int>> adjHLD(N + 1);
    for (int u = 1; u <= N; u++) {
        for (auto [v, w] : adjT_weighted[u]) {
            adjHLD[u].push_back(v);
            adjHLD[v].push_back(u);
        }
    }

    vector<int> depth_hld(N + 1, -1);
    vector<int> sz(N + 1, 0);
    vector<int> heavy(N + 1, -1);
    vector<int> top(N + 1);
    vector<int> in_time(N + 1);
    int timer = 0;

    function<void(int, int)> dfs1 = [&](int u, int p) {
        sz[u] = 1;
        int max_size = 0;
        for (int v : adjHLD[u]) {
            if (v == p) continue;
            depth_hld[v] = depth_hld[u] + 1;
            dfs1(v, u);
            sz[u] += sz[v];
            if (sz[v] > max_size) {
                max_size = sz[v];
                heavy[u] = v;
            }
        }
    };

    depth_hld[1] = 0;
    parent0[1] = 0;
    dfs1(1, 0);

    function<void(int, int)> dfs2 = [&](int u, int tp) {
        top[u] = tp;
        in_time[u] = timer++;
        if (heavy[u] != -1) {
            dfs2(heavy[u], tp);
        }
        for (int v : adjHLD[u]) {
            if (v == parent0[u] || v == heavy[u]) continue;
            dfs2(v, v);
        }
    };

    dfs2(1, 1);

    SegmentTree st(N);

    for (auto [a, b, l] : non_tree_edges) {
        long long w = base_dist[a] + base_dist[b] + l;
        int u = a, v = b;
        while (top[u] != top[v]) {
            if (depth_hld[top[u]] < depth_hld[top[v]]) {
                swap(u, v);
            }
            st.update(1, 0, N - 1, in_time[top[u]], in_time[u], w);
            u = parent0[top[u]];
            if (u == 0) break;
        }
        if (u != 0 && v != 0 && u != v) {
            if (depth_hld[u] > depth_hld[v]) {
                swap(u, v);
            }
            st.update(1, 0, N - 1, in_time[u] + 1, in_time[v], w);
        }
    }

    vector<long long> ans(N + 1, -1);
    for (int i = 2; i <= N; i++) {
        long long candidate = st.query(1, 0, N - 1, in_time[i]);
        if (candidate >= INF) {
            ans[i] = -1;
        } else {
            ans[i] = candidate - base_dist[i];
        }
    }

    for (int i = 2; i <= N; i++) {
        cout << ans[i];
        if (i < N) {
            cout << ' ';
        } else {
            cout << '\n';
        }
    }

    return 0;
}
