#include<bits/stdc++.h>
using namespace std<br>;

const int INF = 1e9;

struct Edge {
    int to, cap, flow, lower;
    int rev;
};

vector<Edge> graph[205];
int n, m;
vector<tuple<int,int,int,int>> edges;
int flow_val[205];

void add_edge(int from, int to, int lower, int cap) {
    graph[from].push_back({to, cap - lower, 0, lower, (int)graph[to].size()});
    graph[to].push_back({from, 0, 0, 0, (int)graph[from].size() - 1});
}

pair<int,vector<int>> solve() {
    cin >> n >> m;
    edges.clear();
    for(int i = 0; i < 205; i++) graph[i].clear();
    
    vector<int> demand(n + 1, 0);
    
    for(int i = 0; i < m; i++) {
        int u, v, l, c;
        cin >> u >> v >> l >> c;
        edges.push_back({u, v, l, c});
        add_edge(u, v, l, c);
        demand[u] += l;
        demand[v] -= l;
    }
    
    // Create super source and super sink
    int source = 0, sink = n + 1;
    for(int i = 1; i <= n; i++) {
        if(demand[i] > 0) {
            add_edge(source, i, 0, demand[i]);
        } else if(demand[i] < 0) {
            add_edge(i, sink, 0, -demand[i]);
        }
    }
    
    // Max flow from source to sink
    auto maxflow = [&](int s, int t) -> int {
        int total = 0;
        while(true) {
            vector<int> parent(n + 2, -1);
            queue<int> q;
            q.push(s);
            parent[s] = s;
            
            while(!q.empty() && parent[t] == -1) {
                int u = q.front(); q.pop();
                for(int i = 0; i < graph[u].size(); i++) {
                    Edge& e = graph[u][i];
                    if(parent[e.to] == -1 && e.cap > e.flow) {
                        parent[e.to] = u * 300 + i;
                        q.push(e.to);
                    }
                }
            }
            
            if(parent[t] == -1) break;
            
            int push = INF;
            for(int v = t; v != s; v = parent[v] / 300) {
                int u = parent[v] / 300;
                int idx = parent[v] % 300;
                push = min(push, graph[u][idx].cap - graph[u][idx].flow);
            }
            
            for(int v = t; v != s; v = parent[v] / 300) {
                int u = parent[v] / 300;
                int idx = parent[v] % 300;
                graph[u][idx].flow += push;
                graph[graph[u][idx].to][graph[u][idx].rev].flow -= push;
            }
            total += push;
        }
        return total;
    };
    
    int total_demand = 0;
    for(int i = 1; i <= n; i++) if(demand[i] > 0) total_demand += demand[i];
    
    if(maxflow(source, sink) != total_demand) {
        return {0, {}};
    }
    
    vector<int> result;
    for(auto [u, v, l, c] : edges) {
        for(auto& e : graph[u]) {
            if(e.to == v && e.lower == l) {
                result.push_back(l + e.flow);
                break;
            }
        }
    }
    
    return {1, result};
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    auto [ok, res] = solve();
    if(!ok) {
        cout << "NO\n";
    } else {
        cout << "YES\n";
        for(int x : res) cout << x << "\n";
    }
    
    return 0;
}
