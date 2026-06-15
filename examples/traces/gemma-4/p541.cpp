#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;

const int MAXN = 100005;
vector<pair<int, int>> g[MAXN];
int deg[MAXN];
bool used_edge[MAXN];
bool removed[MAXN];
int color[MAXN];

struct Path {
    int u, v;
    vector<int> nodes;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back({v, i});
        g[v].push_back({u, i});
        deg[u]++;
        deg[v]++;
    }

    vector<int> junctions;
    for (int i = 1; i <= n; ++i) if (deg[i] == 4) junctions.push_back(i);

    vector<Path> paths;
    for (int i = 1; i <= n; ++i) {
        for (auto& e : g[i]) {
            if (!used_edge[e.second]) {
                int curr = i, next_v = e.first, eid = e.second;
                used_edge[eid] = true;
                vector<int> nodes = {curr};
                while (deg[next_v] == 2) {
                    nodes.push_back(next_v);
                    int prev = curr;
                    curr = next_v;
                    for (auto& ne : g[curr]) {
                        if (ne.second != eid) {
                            eid = ne.second;
                            next_v = ne.first;
                            break;
                        }
                    }
                    used_edge[eid] = true;
                }
                nodes.push_back(next_v);
                paths.push_back({nodes.front(), nodes.back(), nodes});
            }
        }
    }

    map<pair<int, int>, vector<int>> pair_map;
    for (int i = 0; i < (int)paths.size(); ++i) {
        int u = paths[i].u, v = paths[i].v;
        if (u > v) swap(u, v);
        pair_map[{u, v}].push_back(i);
    }

    vector<vector<int>> area_nodes;
    vector<pair<int, int>> area_junctions;
    for (auto const& [nodes, p_indices] : pair_map) {
        for (int i = 0; i < (int)p_indices.size(); i += 2) {
            int p1 = p_indices[i], p2 = p_indices[i+1];
            vector<int> combined = paths[p1].nodes;
            for (int k = (int)paths[p2].nodes.size() - 2; k >= 1; --k)
                combined.push_back(paths[p2].nodes[k]);
            area_nodes.push_back(combined);
            area_junctions.push_back(nodes);
        }
    }

    int num_areas = area_nodes.size();
    vector<vector<int>> area_adj(num_areas);
    for (int i = 0; i < num_areas; ++i) {
        for (int j = i + 1; j < num_areas; ++j) {
            if (area_junctions[i] == area_junctions[j]) {
                area_adj[i].push_back(j);
                area_adj[j].push_back(i);
            }
        }
    }

    vector<int> area_order;
    vector<bool> area_visited(num_areas, false);
    int curr_a = 0;
    for (int i = 0; i < num_areas; ++i) {
        area_order.push_back(curr_a);
        area_visited[curr_a] = true;
        for (int next_a : area_adj[curr_a]) {
            if (!area_visited[next_a]) {
                curr_a = next_a;
                break;
            }
        }
    }

    int k = area_order.size();
    vector<int> odd_in_order(k);
    for (int i = 0; i < k; ++i) odd_in_order[i] = (area_nodes[area_order[i]].size() % 2 != 0);

    int min_total_edges = 1e9;
    vector<int> final_edges(k, 0);
    for (int e0 = 0; e0 < 2; ++e0) {
        for (int ek_1 = 0; ek_1 < 2; ++ek_1) {
            if (odd_in_order[0] == 1 && e0 == 0 && ek_1 == 0) continue;
            vector<vector<int>> dp(k, vector<int>(2, 1e9));
            vector<vector<int>> parent(k, vector<int>(2, -1));
            dp[0][e0] = e0;
            for (int i = 1; i < k - 1; ++i) {
                for (int pe = 0; pe < 2; ++pe) {
                    if (dp[i-1][pe] >= 1e9) continue;
                    for (int ce = 0; ce < 2; ++ce) {
                        if (odd_in_order[i] == 1 && pe == 0 && ce == 0) continue;
                        if (dp[i-1][pe] + ce < dp[i][ce]) {
                            dp[i][ce] = dp[i-1][pe] + ce;
                            parent[i][ce] = pe;
                        }
                    }
                }
            }
            for (int pe = 0; pe < 2; ++pe) {
                if (dp[k-2][pe] >= 1e9) continue;
                if (odd_in_order[k-1] == 1 && pe == 0 && ek_1 == 0) continue;
                if (dp[k-2][pe] + ek_1 < min_total_edges) {
                    min_total_edges = dp[k-2][pe] + ek_1;
                    int cur_e = pe;
                    final_edges[0] = e0; final_edges[k-1] = ek_1;
                    for (int i = k-2; i >= 1; --i) {
                        final_edges[i] = cur_e;
                        cur_e = parent[i][cur_e];
                    }
                    final_edges[0] = e0; // Correctness check
                }
            }
        }
    }

    vector<vector<int>> v_to_a(n + 1);
    for (int i = 0; i < num_areas; ++i) {
        for (int v : area_nodes[i]) v_to_a[v].push_back(i);
    }
    for (int i = 0; i < k; ++i) {
        if (final_edges[i] == 1) {
            int a1 = area_order[i], a2 = area_order[(i+1)%k];
            for (int v : junctions) {
                bool in_a1 = false, in_a2 = false;
                for (int a : v_to_a[v]) { if (a == a1) in_a1 = true; if (a == a2) in_a2 = true; }
                if (in_a1 && in_a2) { removed[v] = true; break; }
            }
        }
    }

    vector<int> s1, s2;
    for (int i = 1; i <= n; ++i) {
        if (!removed[i] && color[i] == 0) {
            queue<int> q; q.push(i); color[i] = 1;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (auto& e : g[u]) {
                    if (!removed[e.first] && color[e.first] == 0) {
                        color[e.first] = 3 - color[u];
                        q.push(e.first);
                    }
                }
            }
        }
    }
    for (int i = 1; i <= n; ++i) {
        if (!removed[i]) {
            if (color[i] == 1) s1.push_back(i);
            else if (color[i] == 2) s2.push_back(i);
        }
    }

    cout << s1.size(); for (int v : s1) cout << " " << v; cout << "\n";
    cout << s2.size(); for (int v : s2) cout << " " << v; cout << "\n";

    return 0;
}

