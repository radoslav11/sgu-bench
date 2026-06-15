#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

struct Edge {
    int to, cap, flow, id;
};

vector<Edge> edges;
vector<int> g[405];
int n, m, s, t;
int level[405], iter[405];

void add_edge(int from, int to, int cap, int id) {
    g[from].push_back(edges.size());
    edges.push_back({to, cap, 0, id});
    g[to].push_back(edges.size());
    edges.push_back({from, 0, 0, -id});
}

bool bfs() {
    fill(level, level + n + 1, -1);
    queue<int> q;
    level[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int id : g[v]) {
            if (level[edges[id].to] < 0 && edges[id].flow < edges[id].cap) {
                level[edges[id].to] = level[v] + 1;
                q.push(edges[id].to);
            }
        }
    }
    return level[t] >= 0;
}

int dfs(int v, int pushed) {
    if (v == t || pushed == 0) return pushed;
    for (int& cid = iter[v]; cid < (int)g[v].size(); cid++) {
        int id = g[v][cid];
        int to = edges[id].to;
        if (level[v] + 1 != level[to] || edges[id].cap <= edges[id].flow)
            continue;
        int tr = dfs(to, min(pushed, edges[id].cap - edges[id].flow));
        if (tr > 0) {
            edges[id].flow += tr;
            edges[id ^ 1].flow -= tr;
            return tr;
        }
    }
    return 0;
}

int max_flow() {
    int flow = 0;
    while (bfs()) {
        fill(iter, iter + n + 1, 0);
        while (int pushed = dfs(s, INF)) {
            flow += pushed;
        }
    }
    return flow;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int m;
    cin >> n >> m >> s >> t;
    
    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        add_edge(u, v, 1, i);
        add_edge(v, u, 1, i);
    }
    
    int flow = max_flow();
    
    vector<vector<int>> crystal_edges(flow + 1);
    
    // Find edge-disjoint paths and assign to crystal types
    for (int crystal = 1; crystal <= flow; crystal++) {
        // BFS to find a path from s to t using available edges
        vector<int> parent(n + 1, -1);
        vector<int> parent_edge(n + 1, -1);
        queue<int> q;
        q.push(s);
        parent[s] = s;
        
        while (!q.empty() && parent[t] == -1) {
            int v = q.front();
            q.pop();
            for (int id : g[v]) {
                int to = edges[id].to;
                if (parent[to] == -1 && edges[id].flow > 0) {
                    parent[to] = v;
                    parent_edge[to] = id;
                    q.push(to);
                }
            }
        }
        
        // Backtrack and collect edges
        int v = t;
        while (v != s) {
            int id = parent_edge[v];
            edges[id].flow--;
            edges[id ^ 1].flow++;
            int edge_id = edges[id].id;
            if (edge_id > 0) {
                crystal_edges[crystal].push_back(edge_id);
            }
            v = parent[v];
        }
    }
    
    cout << flow << "\n";
    for (int i = 1; i <= flow; i++) {
        cout << crystal_edges[i].size();
        for (int e : crystal_edges[i]) {
            cout << " " << e;
        }
        cout << "\n";
    }
    
    return 0;
}
