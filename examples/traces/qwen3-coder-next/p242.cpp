#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

const int INF = 1e9;

struct Dinic {
    struct Edge {
        int from, to, cap, flow;
        Edge(int from, int to, int cap) : from(from), to(to), cap(cap), flow(0) {}
    };

    vector<Edge> edges;
    vector<vector<int>> adj;
    vector<int> level, ptr;
    int n;

    Dinic(int n) : n(n) {
        adj.resize(n);
        level.resize(n);
        ptr.resize(n);
    }

    void add_edge(int from, int to, int cap) {
        adj[from].push_back(edges.size());
        edges.push_back(Edge(from, to, cap));
        adj[to].push_back(edges.size());
        edges.push_back(Edge(to, from, 0));
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int id : adj[u]) {
                Edge& e = edges[id];
                if (level[e.to] == -1 && e.flow < e.cap) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    int dfs(int u, int t, int pushed) {
        if (pushed == 0) return 0;
        if (u == t) return pushed;
        for (int& i = ptr[u]; i < adj[u].size(); i++) {
            int id = adj[u][i];
            Edge& e = edges[id];
            if (level[e.to] == level[u] + 1 && e.flow < e.cap) {
                int how = dfs(e.to, t, min(pushed, e.cap - e.flow));
                if (how) {
                    e.flow += how;
                    edges[id ^ 1].flow -= how;
                    return how;
                }
            }
        }
        return 0;
    }

    int max_flow(int s, int t) {
        int flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (int pushed = dfs(s, t, INF)) {
                flow += pushed;
            }
        }
        return flow;
    }
};

int main() {
    int N, K;
    cin >> N >> K;

    vector<vector<int>> prefs(N);
    set<int> univ_set;
    for (int i = 0; i < N; i++) {
        int cnt;
        cin >> cnt;
        prefs[i].resize(cnt);
        for (int j = 0; j < cnt; j++) {
            cin >> prefs[i][j];
            univ_set.insert(prefs[i][j]);
        }
    }

    vector<int> univ_list(univ_set.begin(), univ_set.end());
    int M = univ_list.size();

    if (M < K || N < 2 * K) {
        cout << "NO" << endl;
        return 0;
    }

    int source = 0;
    int sink = N + M + 2;
    int X_node = N + M + 1;
    int total_nodes = sink + 1;

    Dinic dinic(total_nodes);

    map<int, int> univ_to_idx;
    for (int i = 0; i < M; i++) {
        univ_to_idx[univ_list[i]] = i;
    }

    for (int i = 0; i < N; i++) {
        dinic.add_edge(source, i+1, 1);
    }

    for (int i = 0; i < N; i++) {
        for (int univ : prefs[i]) {
            int idx = univ_to_idx[univ];
            int univ_node = N + 1 + idx;
            dinic.add_edge(i+1, univ_node, 1);
        }
    }

    vector<int> edge_index_to_X(M);
    for (int j = 0; j < M; j++) {
        int univ_node = N + 1 + j;
        int idx_before = dinic.edges.size();
        dinic.add_edge(univ_node, X_node, 2);
        edge_index_to_X[j] = idx_before;
    }

    for (int j = 0; j < M; j++) {
        int univ_node = N + 1 + j;
        dinic.add_edge(univ_node, sink, 1000);
    }

    int idx_edge_X_sink = dinic.edges.size();
    dinic.add_edge(X_node, sink, 2 * K);

    int total_flow = dinic.max_flow(source, sink);
    int flow_on_X_sink = dinic.edges[idx_edge_X_sink].flow;

    if (flow_on_X_sink == 2 * K) {
        cout << "YES" << endl;
        vector<vector<int>> assignment(M);

        for (int i = 0; i < N; i++) {
            int student_node = i+1;
            for (int id : dinic.adj[student_node]) {
                auto& e = dinic.edges[id];
                if (e.to >= N+1 && e.to <= N+M && e.flow == 1) {
                    int univ_idx = e.to - (N+1);
                    assignment[univ_idx].push_back(i+1);
                }
            }
        }

        vector<pair<int, vector<int>>> output_univs;
        for (int j = 0; j < M; j++) {
            int flow_to_X_j = dinic.edges[edge_index_to_X[j]].flow;
            if (flow_to_X_j == 2) {
                output_univs.push_back({univ_list[j], assignment[j]});
            }
        }

        for (auto& p : output_univs) {
            cout << p.second.size();
            for (int student : p.second) {
                cout << " " << student;
            }
            cout << endl;
        }
    } else {
        cout << "NO" << endl;
    }

    return 0;
}
