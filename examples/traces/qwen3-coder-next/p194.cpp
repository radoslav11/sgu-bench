#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

struct Edge {
    int to, cap, flow, index;
    bool is_reverse;
    Edge(int t, int c, int i, bool rev) : to(t), cap(c), flow(0), index(i), is_reverse(rev) {}
};

class Dinic {
public:
    int n;
    vector<vector<Edge*>> adj;
    vector<int> level, ptr;
    
    Dinic(int nodes) : n(nodes), adj(nodes), level(nodes), ptr(nodes) {}
    
    void addEdge(int from, int to, int cap, int index) {
        Edge* forward = new Edge(to, cap, index, false);
        Edge* backward = new Edge(from, 0, index, true);
        adj[from].push_back(forward);
        adj[to].push_back(backward);
    }
    
    bool bfs(int source, int sink) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(source);
        level[source] = 0;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (Edge* e : adj[u]) {
                if (level[e->to] == -1 && e->flow < e->cap) {
                    level[e->to] = level[u] + 1;
                    q.push(e->to);
                }
            }
        }
        return level[sink] != -1;
    }
    
    int dfs(int u, int sink, int flow) {
        if (u == sink || flow == 0) return flow;
        for (int& i = ptr[u]; i < adj[u].size(); i++) {
            Edge* e = adj[u][i];
            if (level[e->to] == level[u] + 1 && e->flow < e->cap) {
                int pushed = dfs(e->to, sink, min(flow, e->cap - e->flow));
                if (pushed > 0) {
                    e->flow += pushed;
                    adj[e->to][e->index]->flow -= pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }
    
    long long maxFlow(int source, int sink) {
        long long total = 0;
        while (bfs(source, sink)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (int pushed = dfs(source, sink, INT_MAX)) {
                total += pushed;
            }
        }
        return total;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<int> l(M), c(M);
    vector<pair<int, int>> edges(M);
    
    // Demand for each node: required_in - required_out
    vector<int> demand(N + 1, 0);
    
    // Read pipes and compute demands
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v >> l[i] >> c[i];
        u--; v--;
        edges[i] = {u, v};
        
        // Lower bound l[i] must be satisfied
        demand[u] -= l[i];
        demand[v] += l[i];
    }
    
    // Create super source and super sink
    int source = N;
    int sink = N + 1;
    Dinic graph(N + 2);
    
    // Connect super source and sink to nodes with imbalanced demands
    long long totalDemand = 0;
    for (int i = 0; i < N; i++) {
        if (demand[i] > 0) {
            graph.addEdge(source, i, demand[i], -1);
            totalDemand += demand[i];
        } else if (demand[i] < 0) {
            graph.addEdge(i, sink, -demand[i], -1);
        }
    }
    
    // Add original pipes with adjusted capacities
    for (int i = 0; i < M; i++) {
        int u = edges[i].first;
        int v = edges[i].second;
        int cap = c[i] - l[i];
        graph.addEdge(u, v, cap, i);
    }
    
    // Compute max flow from super source to super sink
    long long flow = graph.maxFlow(source, sink);
    
    // Check if all demands are satisfied
    if (flow != totalDemand) {
        cout << "NO" << endl;
        return 0;
    }
    
    // Extract flows on original edges
    vector<int> result(M);
    for (int i = 0; i < M; i++) {
        // Forward edge is at index 2*i in adj[u], backward is at index 2*i+1 in adj[v]
        // But since we added other edges too, we need to find the correct edge
        // For each original pipe i, we know it was added as an edge from u to v with index i
        // So look in adj[u] for edge with is_reverse=false and index=i
        int u = edges[i].first;
        for (Edge* e : graph.adj[u]) {
            if (!e->is_reverse && e->index == i) {
                result[i] = e->flow + l[i];
                break;
            }
        }
    }
    
    cout << "YES" << endl;
    for (int i = 0; i < M; i++) {
        cout << result[i] << "\n";
    }
    
    return 0;
}
