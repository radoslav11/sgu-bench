#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>
#include <tuple>
#include <functional>
#include <cstring>
using namespace std;

struct Node {
    int val;
    int max_val;
    int max_id;
    Node *ch[2], *par;
    bool rev;
    Node(int v) : val(v), max_val(v), rev(false), par(nullptr) {
        ch[0] = ch[1] = nullptr;
    }
};

Node* nodes = nullptr;

void update(Node* x) {
    if (!x) return;
    int node_idx = x - nodes;
    x->max_val = x->val;
    x->max_id = (node_idx >= 0 && node_idx >= n ? node_idx - n : -1);
    for (int i = 0; i < 2; i++) {
        if (x->ch[i]) {
            if (x->ch[i]->max_val > x->max_val) {
                x->max_val = x->ch[i]->max_val;
                x->max_id = x->ch[i]->max_id;
            }
        }
    }
}

void push_rev(Node* x) {
    if (!x || !x->rev) return;
    swap(x->ch[0], x->ch[1]);
    if (x->ch[0]) x->ch[0]->rev ^= 1;
    if (x->ch[1]) x->ch[1]->rev ^= 1;
    x->rev = false;
}

void rotate(Node* x) {
    Node* y = x->par;
    if (!y) return;
    int r = (x == y->ch[1]);
    y->ch[r] = x->ch[r ^ 1];
    if (x->ch[r ^ 1]) x->ch[r ^ 1]->par = y;
    x->par = y->par;
    if (y->par) {
        if (y == y->par->ch[0]) y->par->ch[0] = x;
        else if (y == y->par->ch[1]) y->par->ch[1] = x;
    }
    x->ch[r ^ 1] = y;
    y->par = x;
    update(y);
}

void splay(Node* x) {
    push_rev(x);
    while (x->par) {
        Node* y = x->par;
        if (y->par) {
            push_rev(y->par);
            push_rev(y);
            push_rev(x);
            int r = (x == y->ch[1]);
            int r2 = (y == y->par->ch[1]);
            if (r == r2) {
                rotate(y);
                rotate(x);
            } else {
                rotate(x);
                rotate(x);
            }
        } else {
            push_rev(y);
            push_rev(x);
            rotate(x);
        }
    }
    update(x);
}

void access(Node* x) {
    for (Node* y = nullptr; x; x = (y = x)->par) {
        splay(x);
        x->ch[1] = y;
        update(x);
    }
}

void make_root(Node* x) {
    access(x);
    splay(x);
    x->rev ^= 1;
}

void link(Node* x, Node* y) {
    make_root(x);
    x->par = y;
}

void cut(Node* x, Node* y) {
    make_root(x);
    access(y);
    splay(y);
    if (y->ch[0] == x) {
        y->ch[0] = nullptr;
        x->par = nullptr;
        update(y);
    }
}

struct Edge {
    int u, v, w, idx;
};

struct DSU {
    vector<int> parent, rank;
    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    void merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return;
        if (rank[x] < rank[y]) parent[x] = y;
        else if (rank[x] > rank[y]) parent[y] = x;
        else {
            parent[y] = x;
            rank[x]++;
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> edge_u(m), edge_v(m), edge_weight(m);
    for (int i = 0; i < m; i++) {
        cin >> edge_u[i] >> edge_v[i] >> edge_weight[i];
        edge_u[i]--; edge_v[i]--;
    }

    vector<bool> in_mst(m, false);
    vector<Edge> edges;
    for (int i = 0; i < m; i++) {
        edges.push_back({edge_u[i], edge_v[i], edge_weight[i], i});
    }

    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.w < b.w;
    });

    DSU dsu(n);
    long long total_cost = 0;
    for (const Edge& e : edges) {
        if (dsu.find(e.u) != dsu.find(e.v)) {
            dsu.merge(e.u, e.v);
            in_mst[e.idx] = true;
            total_cost += e.w;
        }
    }

    vector<vector<pair<int, int>>> adj(n);
    for (int i = 0; i < m; i++) {
        if (in_mst[i]) {
            adj[edge_u[i]].push_back({edge_v[i], i});
            adj[edge_v[i]].push_back({edge_u[i], i});
        }
    }

    nodes = new Node[n + m];
    for (int i = 0; i < n + m; i++) {
        if (i < n) {
            nodes[i].val = 0;
            nodes[i].max_val = 0;
            nodes[i].max_id = -1;
        } else {
            int eid = i - n;
            nodes[i].val = edge_weight[eid];
            nodes[i].max_val = edge_weight[eid];
            nodes[i].max_id = eid;
        }
    }

    for (int i = 0; i < m; i++) {
        if (in_mst[i]) {
            link(&nodes[edge_u[i]], &nodes[n + i]);
            link(&nodes[edge_v[i]], &nodes[n + i]);
        }
    }

    set<pair<int, int>> non_mst_set;
    for (int i = 0; i < m; i++) {
        if (!in_mst[i]) {
            non_mst_set.insert({edge_weight[i], i});
        }
    }

    int t;
    cin >> t;

    while (t--) {
        int ei, ci;
        cin >> ei >> ci;
        ei--;

        int u = edge_u[ei], v = edge_v[ei];
        int old_weight = edge_weight[ei];
        edge_weight[ei] = ci;

        if (in_mst[ei]) {
            if (ci <= old_weight) {
                make_root(&nodes[n + ei]);
                splay(&nodes[n + ei]);
                nodes[n + ei].val = ci;
                update(&nodes[n + ei]);
                total_cost = total_cost - old_weight + ci;
                cout << total_cost << '\n';
            } else {
                total_cost -= old_weight;

                vector<bool> visited(n, false);
                queue<int> q;
                q.push(u);
                visited[u] = true;
                while (!q.empty()) {
                    int node = q.front(); q.pop();
                    for (auto [neighbor, edge_id] : adj[node]) {
                        if (edge_id == ei) continue;
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }

                int candidate_weight = ci;
                int candidate_edge_id = ei;

                for (auto [w, id] : non_mst_set) {
                    int x = edge_u[id], y = edge_v[id];
                    if (visited[x] != visited[y]) {
                        if (w < candidate_weight) {
                            candidate_weight = w;
                            candidate_edge_id = id;
                        }
                    }
                }

                total_cost += candidate_weight;
                cout << total_cost << '\n';

                in_mst[ei] = false;
                in_mst[candidate_edge_id] = true;

                int x1 = edge_u[candidate_edge_id], y1 = edge_v[candidate_edge_id];
                adj[u].erase(remove(adj[u].begin(), adj[u].end(), make_pair(v, ei)), adj[u].end());
                adj[v].erase(remove(adj[v].begin(), adj[v].end(), make_pair(u, ei)), adj[v].end());
                adj[x1].push_back({y1, candidate_edge_id});
                adj[y1].push_back({x1, candidate_edge_id});

                cut(&nodes[u], &nodes[n + ei]);
                cut(&nodes[v], &nodes[n + ei]);
                link(&nodes[x1], &nodes[n + candidate_edge_id]);
                link(&nodes[y1], &nodes[n + candidate_edge_id]);

                if (candidate_edge_id != ei) {
                    non_mst_set.erase({edge_weight[candidate_edge_id], candidate_edge_id});
                    non_mst_set.insert({edge_weight[ei], ei});
                }
            }
        } else {
            if (ci >= old_weight) {
                non_mst_set.erase({old_weight, ei});
                non_mst_set.insert({ci, ei});
                cout << total_cost << '\n';
            } else {
                make_root(&nodes[u]);
                access(&nodes[v]);
                splay(&nodes[v]);
                int max_weight = nodes[v].max_val;
                int max_edge_id = nodes[v].max_id;

                if (max_weight > ci) {
                    total_cost = total_cost - max_weight + ci;
                    cout << total_cost << '\n';

                    int u1 = edge_u[max_edge_id], v1 = edge_v[max_edge_id];
                    adj[u].push_back({v, ei});
                    adj[v].push_back({u, ei});
                    adj[u1].erase(remove(adj[u1].begin(), adj[u1].end(), make_pair(v1, max_edge_id)), adj[u1].end());
                    adj[v1].erase(remove(adj[v1].begin(), adj[v1].end(), make_pair(u1, max_edge_id)), adj[v1].end());

                    cut(&nodes[u1], &nodes[n + max_edge_id]);
                    cut(&nodes[v1], &nodes[n + max_edge_id]);
                    link(&nodes[u], &nodes[n + ei]);
                    link(&nodes[v], &nodes[n + ei]);

                    non_mst_set.erase({edge_weight[ei], ei});
                    non_mst_set.insert({edge_weight[max_edge_id], max_edge_id});
                    in_mst[ei] = true;
                    in_mst[max_edge_id] = false;
                } else {
                    non_mst_set.erase({old_weight, ei});
                    non_mst_set.insert({ci, ei});
                    cout << total_cost << '\n';
                }
            }
        }
    }

    delete[] nodes;
    return 0;
}
