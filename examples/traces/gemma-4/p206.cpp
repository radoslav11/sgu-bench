#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int to;
    long long cap;
    int rev;
};

vector<vector<Edge>> adj_flow;
vector<int> level;
vector<int> iter;

void add_edge(int from, int to, long long cap) {
    adj_flow[from].push_back({to, cap, (int)adj_flow[to].size()});
    adj_flow[to].push_back({from, 0, (int)adj_flow[from].size() - 1});
}

bool bfs(int s, int t) {
    level.assign(adj_flow.size(), -1);
    level[s] = 0;
    queue<int> que;
    que.push(s);
    while (!que.empty()) {
        int v = que.front();
        que.pop();
        for (auto &e : adj_flow[v]) {
            if (e.cap > 0 && level[e.to] < 0) {
                level[e.to] = level[v] + 1;
                que.push(e.to);
            }
        }
    }
    return level[t] != -1;
}

long long dfs(int v, int t, long long f) {
    if (v == t) return f;
    for (int &i = iter[v]; i < (int)adj_flow[v].size(); ++i) {
        Edge &e = adj_flow[v][i];
        if (e.cap > 0 && level[v] < level[e.to]) {
            long long d = dfs(e.to, t, min(f, e.cap));
            if (d > 0) {
                e.cap -= d;
                adj_flow[e.to][e.rev].cap += d;
                return d;
            }
        }
    }
    return 0;
}

long long max_flow(int s, int t) {
    long long flow = 0;
    while (bfs(s, t)) {
        iter.assign(adj_flow.size(), 0);
        long long f;
        while ((f = dfs(s, t, INF)) > 0) {
            flow += f;
        }
    }
    return flow;
}

struct Road {
    int u, v, c, id;
};

int N, M;
vector<Road> roads;
vector<pair<int, int>> tree_adj[61];
vector<pair<int, int>> constraints;
long long d_values[401];
vector<int> unique_costs;

bool get_path_correct(int u, int target, int p, vector<int> &path) {
    if (u == target) return true;
    for (auto &edge : tree_adj[u]) {
        if (edge.first != p) {
            path.push_back(edge.second);
            if (get_path_correct(edge.first, target, u, path)) return true;
            path.pop_back();
        }
    }
    return false;
}

void solve(vector<int> indices, int L, int R) {
    if (indices.empty()) return;
    if (L == R) {
        for (int idx : indices) d_values[idx] = unique_costs[L];
        return;
    }
    int mid = (L + R) / 2;
    int S = 0, T = (int)indices.size() + 1;
    adj_flow.assign(T + 1, vector<Edge>());
    
    auto get_local_idx = [&](int global_idx) {
        for (int i = 0; i < (int)indices.size(); ++i) {
            if (indices[i] == global_idx) return i + 1;
        }
        return -1;
    };

    for (int i = 0; i < (int)indices.size(); ++i) {
        int idx = indices[i];
        long long w = abs(unique_costs[mid + 1] - roads[idx].c) - abs(unique_costs[mid] - roads[idx].c);
        long long weight = -w;
        if (weight > 0) add_edge(S, i + 1, weight);
        else if (weight < 0) add_edge(i + 1, T, -weight);
    }

    for (auto &con : constraints) {
        int u_idx = get_local_idx(con.first);
        int v_idx = get_local_idx(con.second);
        if (u_idx != -1 && v_idx != -1) {
            add_edge(u_idx, v_idx, INF);
        } else if (u_idx != -1 && v_idx == -1) {
            if (d_values[con.second] <= unique_costs[mid]) add_edge(u_idx, T, INF);
        } else if (u_idx == -1 && v_idx != -1) {
            if (d_values[con.first] >= unique_costs[mid + 1]) add_edge(S, v_idx, INF);
        }
    }

    max_flow(S, T);
    bfs(S, T);
    vector<int> left_indices, right_indices;
    for (int i = 0; i < (int)indices.size(); ++i) {
        if (level[i + 1] != -1) right_indices.push_back(indices[i]);
        else left_indices.push_back(indices[i]);
    }
    solve(left_indices, L, mid);
    solve(right_indices, mid + 1, R);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if (!(cin >> N >> M)) return 0;
    for (int i = 0; i < M; ++i) {
        int u, v, c;
        cin >> u >> v >> c;
        roads.push_back({u, v, c, i});
    }
    for (int i = 0; i < N - 1; ++i) {
        tree_adj[roads[i].u].push_back({roads[i].v, i});
        tree_adj[roads[i].v].push_back({roads[i].u, i});
    }
    for (int i = N - 1; i < M; ++i) {
        vector<int> path;
        get_path_correct(roads[i].u, roads[i].v, 0, path);
        for (int stone_idx : path) constraints.push_back({stone_idx, i});
    }
    set<int> costs_set;
    for (int i = 0; i < M; ++i) costs_set.insert(roads[i].c);
    for (int c : costs_set) unique_costs.push_back(c);
    vector<int> all_indices(M);
    for (int i = 0; i < M; ++i) {
        all_indices[i] = i;
        d_values[i] = unique_costs[0];
    }
    solve(all_indices, 0, (int)unique_costs.size() - 1);
    for (int i = 0; i < M; ++i) cout << (long long)d_values[i] << "\n";
    return 0;
}

