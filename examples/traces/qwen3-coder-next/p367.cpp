#include <bits/stdc++.h>
using namespace std;

void dfs1(int u, vector<bool>& visited, vector<vector<int>>& graph, stack<int>& st) {
    visited[u] = true;
    for (int v : graph[u]) {
        if (!visited[v]) {
            dfs1(v, visited, graph, st);
        }
    }
    st.push(u);
}

void dfs2(int u, vector<bool>& visited, vector<vector<int>>& rev_graph, vector<int>& comp, vector<int>& comp_id, int cid) {
    visited[u] = true;
    comp_id[u] = cid;
    comp.push_back(u);
    for (int v : rev_graph[u]) {
        if (!visited[v]) {
            dfs2(v, visited, rev_graph, comp, comp_id, cid);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N;
    long long T;
    cin >> N >> T;
    vector<int> t(N);
    for (int i = 0; i < N; i++) {
        cin >> t[i];
    }
    int M;
    cin >> M;
    vector<vector<int>> graph(N);
    vector<vector<int>> rev_graph(N);
    for (int i = 0; i < M; i++) {
        int a, b;
        cin >> a >> b;
        a--; b--;
        graph[a].push_back(b);
        rev_graph[b].push_back(a);
    }

    vector<bool> visited1(N, false);
    stack<int> st1;
    for (int i = 0; i < N; i++) {
        if (!visited1[i]) {
            dfs1(i, visited1, graph, st1);
        }
    }

    vector<int> comp_id(N, -1);
    vector<vector<int>> components;
    vector<bool> visited2(N, false);
    int comp_count = 0;

    while (!st1.empty()) {
        int node = st1.top();
        st1.pop();
        if (!visited2[node]) {
            vector<int> comp;
            dfs2(node, visited2, rev_graph, comp, comp_id, comp_count);
            components.push_back(comp);
            comp_count++;
        }
    }

    vector<bool> bad_comp(comp_count, false);
    for (int i = 0; i < comp_count; i++) {
        if (components[i].size() > 1) {
            bad_comp[i] = true;
        }
    }

    vector<unordered_set<int>> cond_set(comp_count);
    for (int u = 0; u < N; u++) {
        for (int v : graph[u]) {
            int c1 = comp_id[u];
            int c2 = comp_id[v];
            if (c1 != c2) {
                cond_set[c1].insert(c2);
            }
        }
    }

    vector<vector<int>> cond_graph(comp_count);
    for (int i = 0; i < comp_count; i++) {
        for (int neighbor : cond_set[i]) {
            cond_graph[i].push_back(neighbor);
        }
    }

    vector<bool> is_bad_comp(comp_count, false);
    queue<int> q;
    for (int i = 0; i < comp_count; i++) {
        if (bad_comp[i]) {
            is_bad_comp[i] = true;
            q.push(i);
        }
    }
    while (!q.empty()) {
        int c = q.front();
        q.pop();
        for (int neighbor : cond_graph[c]) {
            if (!is_bad_comp[neighbor]) {
                is_bad_comp[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    vector<bool> safe(N, false);
    for (int i = 0; i < N; i++) {
        int cid = comp_id[i];
        if (!is_bad_comp[cid] && components[cid].size() == 1) {
            safe[i] = true;
        }
    }

    vector<int> in_degree(N, 0);
    vector<vector<int>> safe_graph(N);
    for (int u = 0; u < N; u++) {
        if (!safe[u]) continue;
        for (int v : graph[u]) {
            if (safe[v]) {
                safe_graph[u].push_back(v);
                in_degree[v]++;
            }
        }
    }

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    for (int i = 0; i < N; i++) {
        if (safe[i] && in_degree[i] == 0) {
            pq.push({t[i], i});
        }
    }

    vector<int> order;
    while (!pq.empty()) {
        auto node = pq.top();
        pq.pop();
        int u = node.second;
        order.push_back(u);
        for (int v : safe_graph[u]) {
            in_degree[v]--;
            if (in_degree[v] == 0) {
                pq.push({t[v], v});
            }
        }
    }

    vector<long long> total_time(order.size() + 1, 0);
    vector<long long> penalty(order.size() + 1, 0);
    for (int k = 1; k <= order.size(); k++) {
        total_time[k] = total_time[k-1] + t[order[k-1]];
        penalty[k] = penalty[k-1] + total_time[k];
    }

    int best_k = 0;
    long long best_penalty = 0;
    for (int k = order.size(); k >= 0; k--) {
        if (total_time[k] <= T) {
            best_k = k;
            best_penalty = penalty[k];
            break;
        }
    }

    cout << best_k << " " << best_penalty << "\n";
    if (best_k > 0) {
        for (int i = 0; i < best_k; i++) {
            if (i > 0) cout << " ";
            cout << order[i] + 1;
        }
    }
    cout << "\n";

    return 0;
}
