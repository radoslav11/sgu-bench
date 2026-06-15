#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Edge {
    int to, rev;
    ll cap;
};

ll dfs(int u, ll flow_in, const vector<int>& levels, vector<vector<Edge>>& graph, vector<int>& ptr, int target) {
    if (u == target) {
        return flow_in;
    }
    ll pushed_total = 0;
    for (int &i = ptr[u]; i < (int)graph[u].size(); i++) {
        Edge &e = graph[u][i];
        if (levels[e.to] == levels[u] + 1 && e.cap > 0) {
            ll pushed = dfs(e.to, min(flow_in - pushed_total, e.cap), levels, graph, ptr, target);
            if (pushed > 0) {
                e.cap -= pushed;
                graph[e.to][e.rev].cap += pushed;
                pushed_total += pushed;
                if (pushed_total == flow_in) {
                    break;
                }
            }
        }
    }
    return pushed_total;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N, M, L;
    cin >> N >> M >> L;

    vector<int> level_node(N+1);
    for (int i = 1; i <= N; i++) {
        cin >> level_node[i];
    }

    int source = -1, target = -1;
    for (int i = 1; i <= N; i++) {
        if (level_node[i] == 1) 
            source = i;
        if (level_node[i] == L)
            target = i;
    }

    vector<vector<Edge>> graph(N+1);
    vector<ll> init_cap(M);
    vector<pair<int, int>> forward_edge_pos(M);

    for (int i = 0; i < M; i++) {
        int a, b;
        ll c;
        cin >> a >> b >> c;
        init_cap[i] = c;

        graph[a].push_back({b, (int)graph[b].size(), c});
        forward_edge_pos[i] = {a, (int)graph[a].size() - 1};

        graph[b].push_back({a, (int)graph[a].size() - 1, 0});
    }

    vector<int> ptr(N+1, 0);
    const ll INF = 1e18;
    dfs(source, INF, level_node, graph, ptr, target);

    for (int i = 0; i < M; i++) {
        int a = forward_edge_pos[i].first;
        int idx = forward_edge_pos[i].second;
        ll flow_val = init_cap[i] - graph[a][idx].cap;
        cout << flow_val << '\n';
    }

    return 0;
}
