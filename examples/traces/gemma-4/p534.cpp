#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

typedef long long ll;

struct Edge {
    int to, id;
    ll t, p;
};

const int MAXN = 100005;
vector<Edge> adj[MAXN];
ll dist_arr[MAXN];
int parent_node[MAXN];
int parent_edge_id[MAXN];
ll parent_edge_p[MAXN];
ll parent_edge_t[MAXN];

void bfs(int start, int n) {
    for (int i = 1; i <= n; ++i) dist_arr[i] = -1;
    queue<int> q;
    q.push(start);
    dist_arr[start] = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto& e : adj[u]) {
            if (dist_arr[e.to] == -1) {
                dist_arr[e.to] = dist_arr[u] + e.t;
                parent_node[e.to] = u;
                parent_edge_id[e.to] = e.id;
                parent_edge_p[e.to] = e.p;
                parent_edge_t[e.to] = e.t;
                q.push(e.to);
            }
        }
    }
}

ll max_dist_in_subtree[MAXN];

void compute_max_dist(int u, int p) {
    max_dist_in_subtree[u] = 0;
    for (auto& e : adj[u]) {
        if (e.to != p) {
            compute_max_dist(e.to, u);
            max_dist_in_subtree[u] = max(max_dist_in_subtree[u], max_dist_in_subtree[e.to] + e.t);
        }
    }
}

struct Result {
    ll cost;
    vector<int> edges;
};

Result solve_cost_fixed(int u, int p, ll d, ll p_edge_p, int p_edge_id) {
    if (d == 0) return {0, {}};

    ll sum_child_costs = 0;
    vector<int> combined_edges;
    bool has_child = false;
    for (auto& e : adj[u]) {
        if (e.to != p && max_dist_in_subtree[e.to] + e.t == d) {
            has_child = true;
            Result res = solve_cost_fixed(e.to, u, max_dist_in_subtree[e.to], e.p, e.id);
            sum_child_costs += res.cost;
            combined_edges.insert(combined_edges.end(), res.edges.begin(), res.edges.end());
        }
    }

    if (!has_child) {
        return {0, {}};
    }

    if (p_edge_id != -1 && p_edge_p < sum_child_costs) {
        return {p_edge_p, {p_edge_id}};
    } else {
        return {sum_child_costs, combined_edges};
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    for (int i = 1; i < n; ++i) {
        int u, v;
        ll t, p;
        cin >> u >> v >> t >> p;
        adj[u].push_back({v, i, t, p});
        adj[v].push_back({u, i, t, p});
    }

    bfs(1, n);
    int u_diam = 1;
    for (int i = 1; i <= n; ++i) if (dist_arr[i] > dist_arr[u_diam]) u_diam = i;
    bfs(u_diam, n);
    int v_diam = 1;
    for (int i = 1; i <= n; ++i) if (dist_arr[i] > dist_arr[v_diam]) v_diam = i;

    ll D = dist_arr[v_diam];
    vector<int> path;
    int curr = v_diam;
    while (curr != u_diam) {
        path.push_back(curr);
        curr = parent_node[curr];
    }
    path.push_back(u_diam);
    reverse(path.begin(), path.end());

    if (D % 2 == 0) {
        ll current_dist = 0;
        int center = -1;
        for (int i = 0; i < (int)path.size(); ++i) {
            if (i > 0) {
                ll t_edge = 0;
                for (auto& e : adj[path[i-1]]) {
                    if (e.to == path[i]) {
                        t_edge = e.t;
                        break;
                    }
                }
                current_dist += t_edge;
            }
            if (current_dist == D / 2) {
                center = path[i];
                break;
            }
        }

        compute_max_dist(center, -1);
        vector<pair<ll, vector<int>>> final_children;
        for (auto& e : adj[center]) {
            if (max_dist_in_subtree[e.to] + e.t == D / 2) {
                Result res = solve_cost_fixed(e.to, center, max_dist_in_subtree[e.to], e.p, e.id);
                final_children.push_back({res.cost, res.edges});
            }
        }
        sort(final_children.begin(), final_children.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

        ll total_cost = 0;
        vector<int> total_edges;
        for (int i = 0; i < (int)final_children.size() - 1; ++i) {
            total_cost += final_children[i].first;
            total_edges.insert(total_edges.end(), final_children[i].second.begin(), final_children[i].second.end());
        }

        cout << total_cost << "\n" << total_edges.size() << "\n";
        for (int i = 0; i < (int)total_edges.size(); ++i) {
            cout << total_edges[i] << (i == (int)total_edges.size() - 1 ? "" : " ");
        }
        cout << endl;

    } else {
        int c1 = -1, c2 = -1;
        ll current_dist = 0;
        for (int i = 0; i < (int)path.size(); ++i) {
            if (i > 0) {
                ll t_edge = 0;
                for (auto& e : adj[path[i-1]]) {
                    if (e.to == path[i]) {
                        t_edge = e.t;
                        break;
                    }
                }
                current_dist += t_edge;
                if (current_dist > D / 2.0 && current_dist - t_edge < D / 2.0) {
                    c1 = path[i-1];
                    c2 = path[i];
                }
            }
        }

        ll p_c1c2 = 0;
        int id_c1c2 = -1;
        for (auto& e : adj[c1]) {
            if (e.to == c2) {
                p_c1c2 = e.p;
                id_c1c2 = e.id;
                break;
            }
        }

        auto solve_all_paths = [&](int root, int other) {
            compute_max_dist(root, other);
            ll d = max_dist_in_subtree[root];
            Result res = solve_cost_fixed(root, other, d, 2000000000LL, -1);
            return res;
        };

        Result res_c1 = solve_all_paths(c1, c2);
        Result res_c2 = solve_all_paths(c2, c1);

        ll min_total_cost = p_c1c2;
        vector<int> min_total_edges = {id_c1c2};

        if (res_c1.cost < min_total_cost) {
            min_total_cost = res_c1.cost;
            min_total_edges = res_c1.edges;
        }
        if (res_c2.cost < min_total_cost) {
            min_total_cost = res_c2.cost;
            min_total_edges = res_c2.edges;
        }

        cout << min_total_cost << "\n" << min_total_edges.size() << "\n";
        for (int i = 0; i < (int)min_total_edges.size(); ++i) {
            cout << min_total_edges[i] << (i == (int)min_total_edges.size() - 1 ? "" : " ");
        }
        cout << endl;
    }

    return 0;
}

