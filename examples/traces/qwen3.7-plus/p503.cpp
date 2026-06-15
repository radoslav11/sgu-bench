#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <tuple>

using namespace std;

struct Edge {
    int to;
    int id;
    long long weight;
};

struct State {
    int fail = 0;
    long long penalty = 0;
    vector<pair<int, int>> children;
    vector<pair<int, int>> go_memo;
};

vector<State> trie(1);

void add_route(const vector<int>& route_edges, long long route_weight) {
    int curr = 0;
    for (int e : route_edges) {
        auto it = lower_bound(trie[curr].children.begin(), trie[curr].children.end(), make_pair(e, -1));
        if (it != trie[curr].children.end() && it->first == e) {
            curr = it->second;
        } else {
            trie[curr].children.insert(it, {e, (int)trie.size()});
            trie.emplace_back();
            curr = trie.size() - 1;
        }
    }
    trie[curr].penalty += route_weight;
}

void build_ac() {
    queue<int> q;
    for (auto& child : trie[0].children) {
        trie[child.second].fail = 0;
        q.push(child.second);
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        trie[u].penalty += trie[trie[u].fail].penalty;
        for (auto& child : trie[u].children) {
            int e = child.first;
            int v = child.second;
            int f = trie[u].fail;
            int next_f = 0;
            while (f != 0) {
                auto it = lower_bound(trie[f].children.begin(), trie[f].children.end(), make_pair(e, -1));
                if (it != trie[f].children.end() && it->first == e) {
                    next_f = it->second;
                    break;
                }
                f = trie[f].fail;
            }
            if (next_f == 0) {
                auto it = lower_bound(trie[0].children.begin(), trie[0].children.end(), make_pair(e, -1));
                if (it != trie[0].children.end() && it->first == e) {
                    next_f = it->second;
                }
            }
            trie[v].fail = next_f;
            q.push(v);
        }
    }
}

int get_go(int u, int e) {
    auto& memo = trie[u].go_memo;
    for (auto& p : memo) {
        if (p.first == e) return p.second;
    }
    int res = 0;
    int curr = u;
    while (curr != 0) {
        auto it = lower_bound(trie[curr].children.begin(), trie[curr].children.end(), make_pair(e, -1));
        if (it != trie[curr].children.end() && it->first == e) {
            res = it->second;
            break;
        }
        curr = trie[curr].fail;
    }
    if (res == 0) {
        auto it = lower_bound(trie[0].children.begin(), trie[0].children.end(), make_pair(e, -1));
        if (it != trie[0].children.end() && it->first == e) {
            res = it->second;
        }
    }
    memo.push_back({e, res});
    return res;
}

struct DistEntry {
    int ac;
    long long dist;
    int p_node;
    int p_ac;
    int p_edge;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, r, S, T;
    if (!(cin >> n >> m >> r >> S >> T)) return 0;

    vector<vector<Edge>> adj(n + 1);
    vector<long long> edge_weights(m + 1, 0);

    for (int i = 1; i <= m; ++i) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, i, w});
        edge_weights[i] = w;
    }

    for (int i = 0; i < r; ++i) {
        int k;
        cin >> k;
        vector<int> route_edges(k);
        long long route_weight = 0;
        for (int j = 0; j < k; ++j) {
            cin >> route_edges[j];
            route_weight += edge_weights[route_edges[j]];
        }
        add_route(route_edges, route_weight);
    }

    build_ac();

    vector<vector<DistEntry>> dist(n + 1);
    
    auto get_dist = [&](int u, int ac) {
        auto& d = dist[u];
        auto it = lower_bound(d.begin(), d.end(), ac, [](const DistEntry& a, int val) {
            return a.ac < val;
        });
        return it;
    };

    dist[S].push_back({0, 0, -1, -1, -1});

    using PQElement = tuple<long long, int, int>;
    priority_queue<PQElement, vector<PQElement>, greater<PQElement>> pq;
    pq.push({0, S, 0});

    bool found = false;
    long long ans_dist = -1;
    int ans_ac = -1;

    while (!pq.empty()) {
        auto [d, u, ac] = pq.top();
        pq.pop();

        auto it = get_dist(u, ac);
        if (it != dist[u].end() && it->dist < d) continue;

        if (u == T) {
            found = true;
            ans_dist = d;
            ans_ac = ac;
            break;
        }

        for (const auto& edge : adj[u]) {
            int v = edge.to;
            int e = edge.id;
            long long w = edge.weight;

            int next_ac = get_go(ac, e);
            long long next_d = d + w + trie[next_ac].penalty;

            auto it_v = get_dist(v, next_ac);
            if (it_v != dist[v].end()) {
                if (next_d < it_v->dist) {
                    it_v->dist = next_d;
                    it_v->p_node = u;
                    it_v->p_ac = ac;
                    it_v->p_edge = e;
                    pq.push({next_d, v, next_ac});
                }
            } else {
                dist[v].insert(it_v, {next_ac, next_d, u, ac, e});
                pq.push({next_d, v, next_ac});
            }
        }
    }

    if (!found) {
        cout << -1 << "\n";
    } else {
        vector<int> path_edges;
        int curr_u = T;
        int curr_ac = ans_ac;
        while (curr_u != S) {
            auto it = get_dist(curr_u, curr_ac);
            path_edges.push_back(it->p_edge);
            int prev_u = it->p_node;
            int prev_ac = it->p_ac;
            curr_u = prev_u;
            curr_ac = prev_ac;
        }
        reverse(path_edges.begin(), path_edges.end());
        cout << ans_dist << "\n";
        cout << path_edges.size() << "\n";
        for (size_t i = 0; i < path_edges.size(); ++i) {
            cout << path_edges[i] << (i + 1 == path_edges.size() ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}
