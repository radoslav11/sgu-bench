#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

struct Edge {
    int from, to;
    int cap, flow;
    Edge(int from, int to, int cap) : from(from), to(to), cap(cap), flow(0) {}
};

vector<Edge> edges;
vector<vector<int>> adj;
vector<int> level;
vector<int> ptr;

void add_edge(int from, int to, int cap) {
    adj[from].push_back(edges.size());
    edges.push_back(Edge(from, to, cap));
    adj[to].push_back(edges.size());
    edges.push_back(Edge(to, from, 0));
}

bool bfs(int S, int T) {
    fill(level.begin(), level.end(), -1);
    queue<int> q;
    q.push(S);
    level[S] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int id : adj[u]) {
            int to = edges[id].to;
            if (level[to] == -1 && edges[id].cap - edges[id].flow > 0) {
                level[to] = level[u] + 1;
                q.push(to);
            }
        }
    }
    return level[T] != -1;
}

int dfs(int u, int flow, int T) {
    if (u == T || !flow) 
        return flow;
    for (int& i = ptr[u]; i < adj[u].size(); i++) {
        int id = adj[u][i];
        int to = edges[id].to;
        if (level[to] == level[u] + 1 && edges[id].cap - edges[id].flow > 0) {
            int pushed = dfs(to, min(flow, edges[id].cap - edges[id].flow), T);
            if (pushed) {
                edges[id].flow += pushed;
                edges[id^1].flow -= pushed;
                return pushed;
            }
        }
    }
    return 0;
}

int dinic(int S, int T) {
    int total_flow = 0;
    while (bfs(S, T)) {
        fill(ptr.begin(), ptr.end(), 0);
        while (int pushed = dfs(S, 1e9, T)) {
            total_flow += pushed;
        }
    }
    return total_flow;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m, S, T;
    cin >> n >> m >> S >> T;
    S--; T--;

    adj.resize(n);
    edges.clear();
    level.resize(n);
    ptr.resize(n);

    vector<pair<int, int>> input_edges;
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--; b--;
        input_edges.push_back({a, b});
        add_edge(a, b, 1);
        add_edge(b, a, 1);
    }

    int lambda = dinic(S, T);
    cout << lambda << "\n";

    for (int iter = 0; iter < lambda; iter++) {
        int flow = dinic(S, T);

        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(S);
        level[S] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int id : adj[u]) {
                int to = edges[id].to;
                if (level[to] == -1 && edges[id].cap - edges[id].flow > 0) {
                    level[to] = level[u] + 1;
                    q.push(to);
                }
            }
        }

        vector<int> cut_list;
        vector<bool> S_side(n, false);
        for (int i = 0; i < n; i++) {
            if (level[i] != -1) 
                S_side[i] = true;
        }

        if (level[T] == -1) {
            for (int j = 0; j < m; j++) {
                int u = input_edges[j].first;
                int v = input_edges[j].second;
                if (edges[2*j].cap - edges[2*j].flow > 0) {
                    cut_list.push_back(j);
                }
            }
        } else {
            for (int j = 0; j < m; j++) {
                int u = input_edges[j].first;
                int v = input_edges[j].second;
                if (S_side[u] != S_side[v]) {
                    if (edges[2*j].cap - edges[2*j].flow > 0) {
                        cut_list.push_back(j);
                    }
                }
            }
        }

        cout << cut_list.size();
        for (int j : cut_list) {
            cout << " " << j+1;
        }
        cout << "\n";

        for (int j : cut_list) {
            edges[2*j].cap = 0;
            edges[2*j].flow = 0;
            edges[2*j+1].cap = 0;
            edges[2*j+1].flow = 0;
        }
    }

    return 0;
}
