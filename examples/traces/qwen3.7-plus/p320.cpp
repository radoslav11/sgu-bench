#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct LT {
    int n;
    vector<int> head, nxt, to;
    int edge_cnt;
    
    vector<int> dfn, vertex, parent, semi, idom, ancestor, label;
    vector<int> bucket_head, bucket_nxt, bucket_to;
    int bucket_cnt;
    int timer;

    LT(int n, int max_edges) : n(n), head(n, -1), nxt(max_edges), to(max_edges), edge_cnt(0),
        dfn(n, 0), vertex(n), parent(n, 0), semi(n), idom(n), ancestor(n), label(n),
        bucket_head(n, -1), bucket_nxt(n), bucket_to(n), bucket_cnt(0), timer(0) {}

    void add_edge(int u, int v) {
        to[edge_cnt] = v;
        nxt[edge_cnt] = head[u];
        head[u] = edge_cnt++;
    }

    void add_bucket(int u, int v) {
        bucket_to[bucket_cnt] = v;
        bucket_nxt[bucket_cnt] = bucket_head[u];
        bucket_head[u] = bucket_cnt++;
    }

    void dfs(int u) {
        timer++;
        dfn[u] = timer;
        vertex[timer] = u;
        semi[u] = u;
        label[u] = u;
        ancestor[u] = u;
        for (int e = head[u]; e != -1; e = nxt[e]) {
            int v = to[e];
            if (!dfn[v]) {
                parent[v] = u;
                dfs(v);
            }
        }
    }

    int find_set(int v) {
        if (ancestor[v] == v) return v;
        int u = find_set(ancestor[v]);
        if (dfn[semi[label[ancestor[v]]]] < dfn[semi[label[v]]]) {
            label[v] = label[ancestor[v]];
        }
        return ancestor[v] = u;
    }

    int eval(int v) {
        find_set(v);
        return label[v];
    }

    void link(int v, int w) {
        ancestor[w] = v;
    }

    void solve(int root) {
        dfs(root);
        for (int i = timer; i >= 2; i--) {
            int w = vertex[i];
            for (int e = head[w]; e != -1; e = nxt[e]) {
                int v = to[e];
                if (dfn[v]) {
                    int u = eval(v);
                    if (dfn[semi[u]] < dfn[semi[w]]) {
                        semi[w] = semi[u];
                    }
                }
            }
            add_bucket(semi[w], w);
            link(parent[w], w);
            
            for (int e = bucket_head[parent[w]]; e != -1; e = bucket_nxt[e]) {
                int v = bucket_to[e];
                int u = eval(v);
                if (dfn[semi[u]] < dfn[v]) {
                    idom[v] = u;
                } else {
                    idom[v] = parent[w];
                }
            }
            bucket_head[parent[w]] = -1;
        }
        for (int i = 2; i <= timer; i++) {
            int w = vertex[i];
            if (idom[w] != semi[w]) {
                idom[w] = idom[idom[w]];
            }
        }
        idom[root] = 0;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M, K;
    if (!(cin >> N >> M >> K)) return 0;

    vector<string> grid(N);
    for (int i = 0; i < N; i++) {
        cin >> grid[i];
    }

    vector<vector<int>> comp_id(N, vector<int>(M, -1));
    vector<int> comp_size;
    int num_comps = 0;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    vector<pair<int, int>> q;
    q.reserve(N * M);

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            if (comp_id[r][c] == -1) {
                char ch = grid[r][c];
                int sz = 0;
                q.clear();
                q.push_back({r, c});
                comp_id[r][c] = num_comps;
                int head_q = 0;
                while (head_q < (int)q.size()) {
                    auto [cr, cc] = q[head_q++];
                    sz++;
                    for (int i = 0; i < 4; i++) {
                        int nr = cr + dr[i];
                        int nc = cc + dc[i];
                        if (nr >= 0 && nr < N && nc >= 0 && nc < M) {
                            if (comp_id[nr][nc] == -1 && grid[nr][nc] == ch) {
                                comp_id[nr][nc] = num_comps;
                                q.push_back({nr, nc});
                            }
                        }
                    }
                }
                comp_size.push_back(sz);
                num_comps++;
            }
        }
    }

    int Border = num_comps;
    int V = num_comps + 1;
    int max_edges = 6 * N * M + 100;
    LT lt(V, max_edges);

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            int u = comp_id[r][c];
            if (r == 0 || r == N - 1 || c == 0 || c == M - 1) {
                lt.add_edge(u, Border);
                lt.add_edge(Border, u);
            }
            if (r + 1 < N) {
                int v = comp_id[r + 1][c];
                if (u != v) {
                    lt.add_edge(u, v);
                    lt.add_edge(v, u);
                }
            }
            if (c + 1 < M) {
                int v = comp_id[r][c + 1];
                if (u != v) {
                    lt.add_edge(u, v);
                    lt.add_edge(v, u);
                }
            }
        }
    }

    lt.solve(Border);

    vector<bool> is_dangerous(V, false);
    for (int i = 0; i < num_comps; i++) {
        if (comp_size[i] >= K) {
            is_dangerous[i] = true;
        }
    }

    for (int i = 2; i <= lt.timer; i++) {
        int w = lt.vertex[i];
        if (lt.idom[w] != 0) {
            is_dangerous[w] = is_dangerous[w] | is_dangerous[lt.idom[w]];
        }
    }

    int ans = 0;
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            if (is_dangerous[comp_id[r][c]]) {
                ans++;
            }
        }
    }

    cout << ans << "\n";

    return 0;
}
