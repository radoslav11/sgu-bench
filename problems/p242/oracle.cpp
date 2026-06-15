#include <bits/stdc++.h>
using namespace std;

// Structure to represent an edge in the flow graph
struct Edge {
    int to;         // endpoint of the edge
    int cap;        // remaining capacity on this edge
    int rev;        // index of the reverse edge in adj[to]
};

struct Dinic {
    int N;                            // total number of nodes
    vector<vector<Edge>> adj;         // adjacency list
    vector<int> level;                // distance from source in level graph
    vector<int> it;                   // iterator for DFS

    Dinic(int n) : N(n), adj(n), level(n), it(n) {}

    // Add directed edge u->v with capacity c, and a reverse edge v->u with capacity 0
    void add_edge(int u, int v, int c) {
        adj[u].push_back({v, c, (int)adj[v].size()});
        adj[v].push_back({u, 0, (int)adj[u].size() - 1});
    }

    // Build level graph by BFS; return true if sink is reachable
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while(!q.empty()) {
            int u = q.front(); q.pop();
            for(auto &e : adj[u]) {
                if(e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }

    // Send flow by DFS in the level graph
    int dfs(int u, int t, int f) {
        if(u == t) return f;
        for(int &i = it[u]; i < (int)adj[u].size(); ++i) {
            Edge &e = adj[u][i];
            if(e.cap > 0 && level[e.to] == level[u] + 1) {
                int ret = dfs(e.to, t, min(f, e.cap));
                if(ret > 0) {
                    e.cap -= ret;                              // reduce forward capacity
                    adj[e.to][e.rev].cap += ret;               // increase reverse capacity
                    return ret;
                }
            }
        }
        return 0;
    }

    // Compute max flow from s to t
    int maxflow(int s, int t) {
        int flow = 0;
        const int INF = 1e9;
        // While we can build a level graph…
        while(bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            // send blocking flows
            while(int pushed = dfs(s, t, INF))
                flow += pushed;
        }
        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;

    // Read preference lists
    vector<vector<int>> pref(N);
    for(int i = 0; i < N; i++) {
        int cnt;
        cin >> cnt;
        pref[i].resize(cnt);
        for(int j = 0; j < cnt; j++) {
            cin >> pref[i][j];
        }
    }

    // Node indexing:
    // S = 0
    // Universities U1..UK at [1 .. K]
    // Students S1..SN at [K+1 .. K+N]
    // T = K+N+1
    int S = 0;
    int uniStart = 1;
    int stuStart = uniStart + K;
    int T = stuStart + N;
    int V = T + 1;

    Dinic dinic(V);

    // Connect source → each university with cap=2
    for(int j = 0; j < K; j++) {
        dinic.add_edge(S, uniStart + j, 2);
    }

    // Connect university j → student i if allowed, cap=1
    for(int i = 0; i < N; i++) {
        for(int u : pref[i]) {
            if(u >= 1 && u <= K) {
                dinic.add_edge(uniStart + (u - 1), stuStart + i, 1);
            }
        }
    }

    // Connect each student → sink with cap=1
    for(int i = 0; i < N; i++) {
        dinic.add_edge(stuStart + i, T, 1);
    }

    // We need total flow = 2*K
    int flow = dinic.maxflow(S, T);
    if(flow < 2 * K) {
        cout << "NO\n";
        return 0;
    }

    cout << "YES\n";
    // Recover assignment: for each university, scan its outgoing edges
    for(int j = 0; j < K; j++) {
        int uNode = uniStart + j;
        vector<int> assigned;
        for(auto &e : dinic.adj[uNode]) {
            // If this was an edge to a student and it is fully used (cap==0),
            // then that student was assigned here.
            if(e.to >= stuStart && e.to < stuStart + N && e.cap == 0) {
                assigned.push_back(e.to - stuStart + 1);
            }
        }
        // Print count and the student ids
        cout << assigned.size();
        for(int sid : assigned) cout << ' ' << sid;
        cout << '\n';
    }

    return 0;
}
