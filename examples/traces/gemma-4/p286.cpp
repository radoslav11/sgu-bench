#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 100005;
const int MAXM = 1000005;

struct Edge {
    int to, id, next;
} edges[2 * MAXM];

int head[MAXN], edge_cnt = 0;
bool used[MAXM];

void add_edge(int u, int v, int id) {
    edges[++edge_cnt] = {v, id, head[u]};
    head[u] = edge_cnt;
}

struct DirectedEdge {
    int u, v, id;
} oriented[MAXM];
int orient_cnt = 0;

int st[MAXN], st_ptr = 0;
int path_u[MAXM], path_v[MAXM], path_id[MAXM], path_ptr = 0;

void find_euler(int start_node) {
    st[st_ptr++] = start_node;
    while (st_ptr > 0) {
        int u = st[st_ptr - 1];
        bool found = false;
        while (head[u] != 0) {
            int e = head[u];
            int v = edges[e].to;
            int id = edges[e].id;
            head[u] = edges[e].next;
            if (used[id]) continue;
            used[id] = true;
            st[st_ptr++] = v;
            path_u[path_ptr] = u;
            path_v[path_ptr] = v;
            path_id[path_ptr] = id;
            path_ptr++;
            found = true;
            break;
        }
        if (!found) {
            if (path_ptr > 0) {
                path_ptr--;
                oriented[orient_cnt++] = {path_u[path_ptr], path_v[path_ptr], path_id[path_ptr]};
            }
            st_ptr--;
        }
    }
}

int head_b[MAXN], to_b[MAXM], edge_id_b[MAXM], next_b[MAXM], edge_cnt_b = 0;
void add_b_edge(int u, int v, int id) {
    to_b[++edge_cnt_b] = v;
    edge_id_b[edge_cnt_b] = id;
    next_b[edge_cnt_b] = head_b[u];
    head_b[u] = edge_cnt_b;
}

int matchL[MAXN], matchR[MAXN], vis[MAXN], timer = 0;
int matched_id[MAXN];

bool dfs(int u) {
    vis[u] = timer;
    for (int e = head_b[u]; e; e = next_b[e]) {
        int v = to_b[e];
        if (matchR[v] == -1 || (vis[matchR[v]] != timer && dfs(matchR[v]))) {
            matchR[v] = u;
            matchL[u] = v;
            matched_id[u] = edge_id_b[e];
            return true;
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    if (!(cin >> N >> K)) return 0;

    int M = N * K / 2;
    for (int i = 1; i <= M; ++i) {
        int u, v;
        cin >> u >> v;
        add_edge(u, v, i);
        add_edge(v, u, i);
    }

    for (int i = 1; i <= N; ++i) {
        if (head[i] != 0) find_euler(i);
    }

    for (int i = 0; i < orient_cnt; ++i) {
        add_b_edge(oriented[i].u, oriented[i].v, oriented[i].id);
    }

    for (int i = 1; i <= N; ++i) {
        matchL[i] = -1;
        matchR[i] = -1;
        vis[i] = 0;
    }

    for (int i = 1; i <= N; ++i) {
        for (int e = head_b[i]; e; e = next_b[e]) {
            int v = to_b[e];
            if (matchR[v] == -1) {
                matchR[v] = i;
                matchL[i] = v;
                matched_id[i] = edge_id_b[e];
                break;
            }
        }
    }

    for (int i = 1; i <= N; ++i) {
        if (matchL[i] == -1) {
            timer++;
            dfs(i);
        }
    }

    cout << "YES" << "\n";
    for (int i = 1; i <= N; ++i) {
        cout << matched_id[i] << "\n";
    }

    return 0;
}

