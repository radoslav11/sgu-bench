#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int from, to, id;
    long long weight;
};

struct TrieNode {
    vector<pair<int, int>> children;
    int fail = 0;
    long long extra_weight = 0;
    int graph_node = 0;
};

const int MAX_L = 200005;
const int MAX_N = 100005;
const int MAX_M = 200005;

TrieNode nodes[MAX_L];
int L_count = 0;
vector<pair<int, int>> memo[MAX_L];

int get_next_st(int st, int edge_id) {
    for (auto& p : memo[st]) {
        if (p.first == edge_id) return p.second;
    }

    int curr = st;
    int next_st = 0;
    while (true) {
        int found_child = 0;
        for (auto& p : nodes[curr].children) {
            if (p.first == edge_id) {
                found_child = p.second;
                break;
            }
        }
        if (found_child != 0) {
            next_st = found_child;
            break;
        }
        if (curr == 0) {
            next_st = 0;
            break;
        }
        curr = nodes[curr].fail;
    }
    memo[st].push_back({edge_id, next_st});
    return next_st;
}

Edge all_edges[MAX_M];
vector<Edge> adj[MAX_N];
long long dist[MAX_N + MAX_L];
int parent_state[MAX_N + MAX_L];
int parent_edge[MAX_N + MAX_L];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, r, S, T;
    if (!(cin >> n >> m >> r >> S >> T)) return 0;

    for (int i = 1; i <= m; ++i) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        all_edges[i] = {u, v, i, w};
        adj[u].push_back(all_edges[i]);
    }

    for (int i = 0; i < r; ++i) {
        int k;
        cin >> k;
        int curr = 0;
        long long route_weight = 0;
        vector<int> route(k);
        for (int j = 0; j < k; ++j) {
            cin >> route[j];
            route_weight += all_edges[route[j]].weight;
        }
        for (int edge_id : route) {
            int found_child = 0;
            for (auto& p : nodes[curr].children) {
                if (p.first == edge_id) {
                    found_child = p.second;
                    break;
                }
            }
            if (found_child == 0) {
                L_count++;
                nodes[L_count].graph_node = all_edges[edge_id].to;
                nodes[curr].children.push_back({edge_id, L_count});
                curr = L_count;
            } else {
                curr = found_child;
            }
        }
        nodes[curr].extra_weight += route_weight;
    }

    queue<int> q;
    for (auto& p : nodes[0].children) {
        q.push(p.second);
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto& p : nodes[u].children) {
            int edge_id = p.first;
            int v = p.second;
            int f = nodes[u].fail;
            int next_f = 0;
            while (true) {
                int found_f_child = 0;
                for (auto& pf : nodes[f].children) {
                    if (pf.first == edge_id) {
                        found_f_child = pf.second;
                        break;
                    }
                }
                if (found_f_child != 0) {
                    next_f = found_f_child;
                    break;
                }
                if (f == 0) {
                    next_f = 0;
                    break;
                }
                f = nodes[f].fail;
            }
            nodes[v].fail = next_f;
            nodes[v].extra_weight += nodes[next_f].extra_weight;
            q.push(v);
        }
    }

    int total_states = n + L_count;
    for (int i = 0; i <= total_states; ++i) dist[i] = INF;

    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

    dist[S] = 0;
    pq.push({0, S});

    while (!pq.empty()) {
        pair<long long, int> top = pq.top();
        pq.pop();
        long long d = top.first;
        int s = top.second;

        if (d > dist[s]) continue;

        int u, st;
        if (s <= n) {
            u = s;
            st = 0;
        } else {
            st = s - n;
            u = nodes[st].graph_node;
        }

        for (auto& e : adj[u]) {
            int next_st = get_next_st(st, e.id);
            int next_s = (next_st == 0 ? e.to : n + next_st);
            long long next_dist = d + e.weight + nodes[next_st].extra_weight;

            if (next_dist < dist[next_s]) {
                dist[next_s] = next_dist;
                parent_state[next_s] = s;
                parent_edge[next_s] = e.id;
                pq.push({next_dist, next_s});
            }
        }
    }

    long long min_dist = INF;
    int best_s = -1;
    for (int s = 1; s <= n; ++s) {
        if (s == T && dist[s] < min_dist) {
            min_dist = dist[s];
            best_s = s;
        }
    }
    for (int s = n + 1; s <= n + L_count; ++s) {
        if (nodes[s - n].graph_node == T && dist[s] < min_dist) {
            min_dist = dist[s];
            best_s = s;
        }
    }

    if (best_s == -1) {
        cout << -1 << endl;
    } else {
        cout << min_dist << endl;
        vector<int> path;
        int curr = best_s;
        while (curr != S || dist[curr] != 0) {
            if (dist[curr] == 0) break;
            path.push_back(parent_edge[curr]);
            curr = parent_state[curr];
            if (curr == S && dist[curr] == 0) break;
        }
        reverse(path.begin(), path.end());
        cout << path.size() << endl;
        for (int i = 0; i < (int)path.size(); ++i) {
            cout << path[i] << (i == (int)path.size() - 1 ? "" : " ");
        }
        cout << endl;
    }

    return 0;
}

