#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

struct Edge { int to, cap, rev; };
vector<Edge> graph[500];
int level[500], iter[500];

void add_edge(int from, int to, int cap) {
    graph[from].push_back({to, cap, (int)graph[to].size()});
    graph[to].push_back({from, 0, (int)graph[from].size()-1});
}

bool bfs(int s, int t, int V) {
    fill(level, level+V, -1);
    queue<int> q;
    level[s] = 0; q.push(s);
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (auto& e : graph[v]) if (e.cap > 0 && level[e.to] < 0) { level[e.to] = level[v]+1; q.push(e.to); }
    }
    return level[t] >= 0;
}

int dfs(int v, int t, int f) {
    if (v == t) return f;
    for (int& i = iter[v]; i < (int)graph[v].size(); i++) {
        Edge& e = graph[v][i];
        if (e.cap > 0 && level[v] < level[e.to]) {
            int d = dfs(e.to, t, min(f, e.cap));
            if (d > 0) { e.cap -= d; graph[e.to][e.rev].cap += d; return d; }
        }
    }
    return 0;
}

int max_flow(int s, int t, int V) {
    int flow = 0;
    while (bfs(s, t, V)) { fill(iter, iter+V, 0); int d; while ((d=dfs(s,t,INF))>0) flow+=d; }
    return flow;
}

int main(){
    int N; cin>>N;
    vector<int> w(N), r(N);
    for (int i=0;i<N;i++) cin>>w[i];
    for (int i=0;i<N;i++) cin>>r[i];
    vector<vector<int>> a(N, vector<int>(N));
    for (int i=0;i<N;i++) for (int j=0;j<N;j++) cin>>a[i][j];
    
    int W = w[0] + r[0];
    for (int i=1;i<N;i++) if (w[i] > W) { cout<<"NO\n"; return 0; }
    
    // nodes: source=0, sink=1, teams 2..N+1, game nodes start at N+2
    int S=0, T=1;
    int total_nodes = 2 + (N-1) + (N-1)*(N-2)/2;
    for (int i=0;i<total_nodes;i++) graph[i].clear();
    
    int team_node[20];
    for (int i=1;i<N;i++) team_node[i] = 2+(i-1);
    int gnode = 2+(N-1);
    
    for (int i=1;i<N;i++) add_edge(team_node[i], T, W - w[i]);
    
    int total = 0;
    for (int i=1;i<N;i++) for (int j=i+1;j<N;j++) {
        if (a[i][j] > 0) {
            add_edge(S, gnode, a[i][j]);
            add_edge(gnode, team_node[i], a[i][j]);
            add_edge(gnode, team_node[j], a[i][j]);
            total += a[i][j];
        }
        gnode++;
    }
    
    cout << (max_flow(S, T, total_nodes) == total ? "YES" : "NO") << "\n";
}
