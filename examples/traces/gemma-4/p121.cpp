#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Edge {
    int to;
    int id;
};

const int MAXN = 105;
vector<Edge> adj[MAXN];
int edge_color[MAXN * MAXN];
bool visited[MAXN];
int parent_node[MAXN], p_edge[MAXN];
vector<int> post_order;

void dfs(int u, int p, int pe) {
    visited[u] = true;
    parent_node[u] = p;
    p_edge[u] = pe;
    for (auto& e : adj[u]) {
        if (e.to == p) continue;
        if (visited[e.to]) {
            if (edge_color[e.id] == 0) edge_color[e.id] = 1;
        } else {
            dfs(e.to, u, e.id);
        }
    }
    post_order.push_back(u);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int N;
    if (!(cin >> N)) return 0;

    vector<vector<int>> original_adj(N + 1);
    for (int i = 1; i <= N; ++i) {
        int v;
        while (cin >> v && v != 0) original_adj[i].push_back(v);
    }

    vector<pair<int, int>> adj_with_id[MAXN];
    int edge_id_counter = 0;
    for (int i = 1; i <= N; ++i) {
        for (int neighbor : original_adj[i]) {
            if (i < neighbor) {
                adj_with_id[i].push_back({neighbor, edge_id_counter});
                adj_with_id[neighbor].push_back({i, edge_id_counter});
                edge_id_counter++;
            }
        }
    }

    for (int i = 1; i <= N; ++i) {
        for (auto& p : adj_with_id[i]) {
            adj[i].push_back({p.first, p.second});
        }
    }

    vector<bool> comp_visited(N + 1, false);
    for (int i = 1; i <= N; ++i) {
        if (!comp_visited[i] && !adj[i].empty()) {
            vector<int> component;
            vector<int> q = {i};
            comp_visited[i] = true;
            int head = 0;
            while(head < (int)q.size()) {
                int u = q[head++];
                component.push_back(u);
                for(auto& e : adj[u]) if(!comp_visited[e.to]) { comp_visited[e.to] = true; q.push_back(e.to); }
            }
            bool is_odd_cycle = true;
            for (int v : component) if (adj[v].size() != 2) { is_odd_cycle = false; break; }
            if (is_odd_cycle && component.size() % 2 != 0) {
                cout << "No solution" << endl;
                return 0;
            }
            int root = component[0];
            for (int v : component) if (adj[v].size() != 2) { root = v; break; }
            post_order.clear();
            for (int v : component) visited[v] = false;
            dfs(root, -1, -1);
            for (int u : post_order) {
                if (u == root) continue;
                int pe = p_edge[u];
                int has1 = 0, has2 = 0;
                for (auto& e : adj[u]) {
                    if (edge_color[e.id] == 1) has1++;
                    else if (edge_color[e.id] == 2) has2++;
                }
                if (adj[u].size() > 1) {
                    if (has1 > 0 && has2 > 0) edge_color[pe] = 1;
                    else if (has1 > 0) edge_color[pe] = 2;
                    else if (has2 > 0) edge_color[pe] = 1;
                    else edge_color[pe] = 1;
                } else edge_color[pe] = 1;
            }
            if (adj[root].size() >= 2) {
                int has1 = 0, has2 = 0;
                for (auto& e : adj[root]) {
                    if (edge_color[e.id] == 1) has1++;
                    else if (edge_color[e.id] == 2) has2++;
                }
                if (has1 == 0 || has2 == 0) {
                    for (auto& e : adj[root]) {
                        int old_c = edge_color[e.id];
                        edge_color[e.id] = (old_c == 1 ? 2 : 1);
                        int r_h1 = 0, r_h2 = 0;
                        for (auto& re : adj[root]) {
                            if (edge_color[re.id] == 1) r_h1++;
                            else if (edge_color[re.id] == 2) r_h2++;
                        }
                        int c_h1 = 0, c_h2 = 0;
                        for (auto& ce : adj[e.to]) {
                            if (edge_color[ce.id] == 1) c_h1++;
                            else if (edge_color[ce.id] == 2) c_h2++;
                        }
                        if (r_h1 > 0 && r_h2 > 0 && (adj[e.to].size() <= 1 || (c_h1 > 0 && c_h2 > 0))) break;
                        else edge_color[e.id] = old_c;
                    }
                }
            }
            for(int v : component) visited[v] = true;
        }
    }
    for (int i = 1; i <= N; ++i) {
        for (auto& p : adj_with_id[i]) cout << (edge_color[p.second] == 1 ? "1 " : "2 ");
        cout << "0" << endl;
    }
    return 0;
}

