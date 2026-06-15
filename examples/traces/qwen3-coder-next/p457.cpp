#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <tuple>
#include <climits>

using namespace std;

const int INF = 1e9;

struct Dinic {
    struct Edge {
        int to, capacity, flow;
        Edge(int to, int capacity) : to(to), capacity(capacity), flow(0) {}
    };
    
    vector<vector<int>> adj;
    vector<Edge> edges;
    vector<int> level, ptr;
    int n;
    
    Dinic(int n) : n(n) {
        adj.resize(n);
        level.resize(n);
        ptr.resize(n);
    }
    
    void add_edge(int u, int v, int capacity) {
        adj[u].push_back(edges.size());
        edges.push_back(Edge(v, capacity));
        adj[v].push_back(edges.size());
        edges.push_back(Edge(u, 0));
    }
    
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(s);
        level[s] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int idx : adj[u]) {
                Edge& e = edges[idx];
                if (e.capacity - e.flow > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }
    
    int dfs(int u, int t, int flow) {
        if (u == t || flow == 0)
            return flow;
        int pushed_total = 0;
        for (int& i = ptr[u]; i < (int)adj[u].size(); i++) {
            int idx = adj[u][i];
            Edge& e = edges[idx];
            if (level[e.to] == level[u] + 1 && e.capacity - e.flow > 0) {
                int pushed = dfs(e.to, t, min(flow, e.capacity - e.flow));
                if (pushed > 0) {
                    e.flow += pushed;
                    edges[idx^1].flow -= pushed;
                    pushed_total += pushed;
                    flow -= pushed;
                    if (flow == 0)
                        break;
                }
            }
        }
        return pushed_total;
    }
    
    int max_flow(int s, int t) {
        int total_flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (int pushed = dfs(s, t, INF)) {
                total_flow += pushed;
            }
        }
        return total_flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int n, m, A, B;
    cin >> n >> m >> A >> B;
    A--; B--;
    
    vector<tuple<int, int, int, int>> roads;
    for (int i = 0; i < m; i++) {
        int xi, yi, wi, ti;
        cin >> xi >> yi >> wi >> ti;
        xi--; yi--;
        roads.push_back(make_tuple(xi, yi, wi, ti));
    }
    
    vector<int> D(n, 0);
    for (int i = 0; i < m; i++) {
        auto [u, v, w, t] = roads[i];
        int L = (t == 1) ? w : 0;
        D[u] -= L;
        D[v] += L;
    }
    
    int total_demand = 0;
    for (int i = 0; i < n; i++) {
        if (D[i] > 0) {
            total_demand += D[i];
        }
    }
    
    int N = n + 2;
    int S_node = n;
    int T_node = n + 1;
    Dinic dinic(N);
    
    vector<int> road_forward_index(m);
    for (int i = 0; i < m; i++) {
        auto [u, v, w, t] = roads[i];
        int L = (t == 1) ? w : 0;
        int cap = w - L;
        road_forward_index[i] = dinic.edges.size();
        dinic.add_edge(u, v, cap);
    }
    
    dinic.add_edge(B, A, INF);
    
    for (int i = 0; i < n; i++) {
        if (D[i] > 0) {
            dinic.add_edge(S_node, i, D[i]);
        } else if (D[i] < 0) {
            dinic.add_edge(i, T_node, -D[i]);
        }
    }
    
    int flow_ST = dinic.max_flow(S_node, T_node);
    if (flow_ST != total_demand) {
        cout << 0 << endl;
        return 0;
    }
    
    int F1 = dinic.edges[2 * m].flow;
    
    int F2 = dinic.max_flow(A, B);
    int F = F1 + F2;
    
    vector<vector<int>> rem(n, vector<int>(n, 0));
    for (int i = 0; i < m; i++) {
        auto [u, v, w, t] = roads[i];
        int L = (t == 1) ? w : 0;
        int g = dinic.edges[road_forward_index[i]].flow;
        int f = L + g;
        rem[u][v] = f;
    }
    
    vector<vector<int>> paths;
    for (int i = 0; i < F; i++) {
        vector<int> parent(n, -1);
        queue<int> q;
        q.push(A);
        parent[A] = -2;
        
        while (!q.empty() && parent[B] == -1) {
            int u = q.front(); q.pop();
            for (int v = 0; v < n; v++) {
                if (rem[u][v] > 0 && parent[v] == -1) {
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
        
        if (parent[B] == -1) {
            break;
        }
        
        vector<int> path;
        int cur = B;
        while (cur != -2) {
            path.push_back(cur);
            cur = parent[cur];
        }
        reverse(path.begin(), path.end());
        
        paths.push_back(path);
        
        for (int j = 0; j < (int)path.size() - 1; j++) {
            rem[path[j]][path[j+1]]--;
        }
    }
    
    cout << F << endl;
    for (auto& path : paths) {
        for (int j = 0; j < (int)path.size(); j++) {
            if (j > 0) cout << " ";
            cout << path[j] + 1;
        }
        cout << endl;
    }
    
    return 0;
}
