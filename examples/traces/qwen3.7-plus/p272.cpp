#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>

using namespace std;

const int INF = 1e9;
int head[20005], to[300005], cap[300005], flow[300005], nxt[300005];
int edge_cnt = 0;

void add_edge(int u, int v, int c) {
    to[edge_cnt] = v;
    cap[edge_cnt] = c;
    flow[edge_cnt] = 0;
    nxt[edge_cnt] = head[u];
    head[u] = edge_cnt++;
    
    to[edge_cnt] = u;
    cap[edge_cnt] = 0;
    flow[edge_cnt] = 0;
    nxt[edge_cnt] = head[v];
    head[v] = edge_cnt++;
}

int level[20005];
int ptr[20005];

bool bfs(int s, int t) {
    memset(level, -1, sizeof(level));
    level[s] = 0;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int e = head[v]; e != -1; e = nxt[e]) {
            if (cap[e] - flow[e] > 0 && level[to[e]] == -1) {
                level[to[e]] = level[v] + 1;
                q.push(to[e]);
            }
        }
    }
    return level[t] != -1;
}

int dfs(int v, int t, int pushed) {
    if (pushed == 0) return 0;
    if (v == t) return pushed;
    for (int& e = ptr[v]; e != -1; e = nxt[e]) {
        int tr = to[e];
        if (level[v] + 1 != level[tr] || cap[e] - flow[e] == 0) continue;
        int push = dfs(tr, t, min(pushed, cap[e] - flow[e]));
        if (push == 0) continue;
        flow[e] += push;
        flow[e ^ 1] -= push;
        return push;
    }
    return 0;
}

int dinic(int s, int t) {
    int max_flow = 0;
    while (bfs(s, t)) {
        memcpy(ptr, head, sizeof(head));
        while (int pushed = dfs(s, t, INF)) {
            max_flow += pushed;
        }
    }
    return max_flow;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    vector<vector<int>> adj(N + 1);
    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int nA;
    cin >> nA;
    vector<int> A(nA);
    vector<bool> isA(N + 1, false);
    for (int i = 0; i < nA; ++i) {
        cin >> A[i];
        isA[A[i]] = true;
    }

    int nB;
    cin >> nB;
    vector<int> B(nB);
    vector<bool> isB(N + 1, false);
    for (int i = 0; i < nB; ++i) {
        cin >> B[i];
        isB[B[i]] = true;
    }

    vector<int> distA(N + 1, -1);
    queue<int> q;
    for (int u : A) {
        distA[u] = 0;
        q.push(u);
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (distA[v] == -1) {
                distA[v] = distA[u] + 1;
                q.push(v);
            }
        }
    }

    vector<int> distB(N + 1, -1);
    queue<int> qB;
    for (int u : B) {
        distB[u] = 0;
        qB.push(u);
    }
    while (!qB.empty()) {
        int u = qB.front();
        qB.pop();
        for (int v : adj[u]) {
            if (distB[v] == -1) {
                distB[v] = distB[u] + 1;
                qB.push(v);
            }
        }
    }

    int K = -1;
    for (int u : B) {
        if (distA[u] != -1) {
            if (K == -1 || distA[u] < K) {
                K = distA[u];
            }
        }
    }

    memset(head, -1, sizeof(head));
    edge_cnt = 0;

    int S = 0;
    int T = 2 * N + 1;

    for (int u : A) {
        add_edge(S, u, 1);
    }

    for (int u : B) {
        add_edge(u + N, T, 1);
    }

    for (int u = 1; u <= N; ++u) {
        if (distA[u] != -1 && distB[u] != -1 && distA[u] + distB[u] == K) {
            add_edge(u, u + N, 1);
        }
    }

    vector<int> seen(N + 1, 0);
    int seen_version = 0;
    for (int u = 1; u <= N; ++u) {
        if (distA[u] != -1 && distB[u] != -1 && distA[u] + distB[u] == K) {
            seen_version++;
            for (int v : adj[u]) {
                if (seen[v] == seen_version) continue;
                seen[v] = seen_version;
                if (distA[v] == distA[u] + 1 && distA[v] + distB[v] == K) {
                    add_edge(u + N, v, 1);
                }
            }
        }
    }

    dinic(S, T);

    vector<vector<int>> paths;
    for (int u : A) {
        bool used = false;
        for (int e = head[S]; e != -1; e = nxt[e]) {
            if (to[e] == u && flow[e] == 1) {
                used = true;
                break;
            }
        }
        if (!used) continue;

        vector<int> path;
        int curr = u;
        path.push_back(curr);
        while (true) {
            if (isB[curr]) {
                bool reached_T = false;
                for (int e = head[curr + N]; e != -1; e = nxt[e]) {
                    if (to[e] == T && flow[e] == 1) {
                        reached_T = true;
                        break;
                    }
                }
                if (reached_T) {
                    paths.push_back(path);
                    break;
                }
            }

            int next_u = -1;
            for (int e = head[curr + N]; e != -1; e = nxt[e]) {
                if (to[e] >= 1 && to[e] <= N && flow[e] == 1) {
                    next_u = to[e];
                    break;
                }
            }
            if (next_u == -1) {
                break;
            }
            curr = next_u;
            path.push_back(curr);
        }
    }

    cout << paths.size() << " " << K << "\n";
    for (const auto& path : paths) {
        for (int i = 0; i < (int)path.size(); ++i) {
            cout << path[i] << (i + 1 == (int)path.size() ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}
